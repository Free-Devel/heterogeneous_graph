//cpu_resource.hpp
#pragma once
#include <queue>
#include <map>
#include <mutex>
#include <condition_variable>
#include <boost/config.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/latch.hpp>
#include <boost/lockfree/queue.hpp>
#include "resource.hpp"

namespace heterogeneous_graph {

extern const boost::uuids::uuid CPU_UUID;

class calculation_task;
class cpu_resource;

namespace detail {

class cpu_worker
{
   typedef boost::lockfree::queue<calculation_task *> ready_tasks_type;
public:
   cpu_worker(
      std::mutex &ready_mutex,
      ready_tasks_type &ready_tasks,
      const bool &stop,
      std::condition_variable &ready_cond,
      boost::latch &end
   );

   void operator()();

private:
   std::mutex &_ready_mutex;
   ready_tasks_type &_ready_tasks;
   const bool &_stop;
   std::condition_variable &_ready_cond;
   boost::latch &_end;
};

} // namespace detail

class cpu_resource: public resource
{
public:
   cpu_resource();
   cpu_resource(size_t n_workers);
   virtual ~cpu_resource();

   void move_to_ready(calculation_task *ct);
   void stop();

private:
   typedef boost::lockfree::queue<calculation_task *> ready_tasks_type;
   ready_tasks_type _ready_tasks;

   const size_t _n_workers;
   bool _stop;
   std::condition_variable _ready_cond;
   std::mutex _ready_mutex;
   std::mutex _locked_workers_mutex;
   boost::thread_group _pool;
   boost::latch _end;

   friend class detail::cpu_worker;
};

} // namespace heterogeneous_graph
