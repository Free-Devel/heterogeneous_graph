//cpu_resource.cpp
#ifdef WIN32
#include "windows.h"
#endif // WIN32
#include <thread>
#include <chrono>
#include <boost/thread/mutex.hpp>
#include <boost/uuid/random_generator.hpp>
#include "cpu_resource.hpp"
#include "calculation_task.hpp"

namespace heterogeneous_graph {

const boost::uuids::uuid CPU_UUID = boost::uuids::random_generator()();

detail::cpu_worker::cpu_worker(
   std::mutex &ready_mutex,
   ready_tasks_type &ready_tasks,
   const bool &stop,
   std::condition_variable &ready_cond,
   boost::latch &end
): _ready_mutex(ready_mutex),
   _ready_tasks(ready_tasks),
   _stop(stop),
   _ready_cond(ready_cond),
   _end(end)
{
}

void detail::cpu_worker::operator()()
{
   try {
      calculation_task::ptr ct;
      while(!_stop) {
         {
            std::unique_lock<std::mutex> lock(_ready_mutex);
            while(_ready_tasks.empty() && !_stop)
               _ready_cond.wait_for(lock, std::chrono::milliseconds(1));
         }

         if(_stop)
            break;

         _ready_tasks.pop(ct);

         (*ct).run();
      }
   }
   catch(...) {
      std::cout << "ERROR" << std::endl;
   }
   _end.count_down();
}

cpu_resource::cpu_resource(
): resource(CPU_UUID),
   _stop(false),
   _n_workers(std::thread::hardware_concurrency()),
   _end(std::thread::hardware_concurrency()),
   _ready_tasks(1024)
{
   for(size_t i = 0; i < _n_workers; ++i) {
      boost::thread *th = _pool.create_thread(
         detail::cpu_worker(
            _ready_mutex,
            _ready_tasks,
            _stop,
            _ready_cond,
            _end
         )
      );

#ifdef NDEBUG
#ifdef WIN32
   HANDLE handle = th->native_handle();
   BOOL res = SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST);
   if(!SetThreadAffinityMask(handle, 1 << i))
      throw std::runtime_error("Impossible to set thread affinity.");
#endif // WIN32
#endif // NDEBUG
   }
}

cpu_resource::cpu_resource(
   size_t n_workers
): resource(CPU_UUID),
   _stop(false),
   _n_workers(n_workers),
   _end(n_workers),
   _ready_tasks(1024)
{
   for(size_t i = 0; i < _n_workers; ++i) {
      boost::thread *th = _pool.create_thread(
         detail::cpu_worker(
            _ready_mutex,
            _ready_tasks,
            _stop,
            _ready_cond,
            _end
         )
      );

#ifdef WIN32
#ifdef NDEBUG
   HANDLE handle = th->native_handle();
   BOOL res = SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST);
#endif // NDEBUG
#endif // WIN32
   }
}

cpu_resource::~cpu_resource()
{
   _end.wait();
}

void cpu_resource::move_to_ready(calculation_task *ct)
{
   _ready_tasks.push(ct);
   _ready_cond.notify_one();
}

void cpu_resource::stop()
{
   _stop = true;
}

} // namespace heterogeneous_graph 
