//calculation_task.cpp
#include <mutex>
#include <boost/config.hpp>
#include <boost/lexical_cast.hpp>
#include "calculation_task.hpp"
#include "utility_stuff.hpp"
#include "counter.hpp"
#include "cpu_resource.hpp"
#include "types.hpp"

namespace heterogeneous_graph {

calculation_task::calculation_task(
   environment &env
): resource_(env.get_resource(CPU_UUID)),
   environment_(env),
   uuid_(boost::uuids::random_generator()()),
   end_(false)
{
}


calculation_task::~calculation_task()
{
}

boost::uuids::uuid calculation_task::uuid() const
{
   return uuid_;
}

void calculation_task::set_input(const data_vector &input)
{
   in_out_ = input;
}

data_vector calculation_task::get_output() const
{
   std::unique_lock<std::mutex> lock(end_mtx_);
   if(!end_)
      cond_.wait(lock);

   return in_out_;
}

void calculation_task::move_to_ready() const
{
   resource_.move_to_ready(const_cast<calculation_task *>(this));
}

} // namespace heterogeneous_graph
