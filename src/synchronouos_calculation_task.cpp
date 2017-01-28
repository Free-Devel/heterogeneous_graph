// synchronouos_calculation_task.cpp
#include <mutex>
#include <boost/config.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/any.hpp>
#include "synchronouos_calculation_task.hpp"
#include "utility_stuff.hpp"
#include "counter.hpp"
#include "multi_call_counter.hpp"

namespace heterogeneous_graph {
namespace vk = vertex_kind;

void synchronouos_calculation_task::init()
{
   end_ = false;

   n_in_ = boost::in_degree(vertex_, graph_);
   boost::tie(in_begin_, in_end_) = boost::in_edges(vertex_, graph_);
   counters_[vertex_].reset(new counter(n_in_));

   n_out_ = boost::out_degree(vertex_, graph_);
   boost::tie(out_begin_, out_end_) = boost::out_edges(vertex_, graph_);
}

bool synchronouos_calculation_task::is_ready() const
{
   return counters_[vertex_]->is_ready();
}

void synchronouos_calculation_task::run()
{
   if(n_in_) {
      in_out_.resize(n_in_);
      size_t i = 0;
      for(auto pos = in_begin_; pos != in_end_; ++pos, ++i)
         in_out_[i] = datas_[*pos];
   }

   in_out_ = stuff_->run(in_out_, environment_);

   dispach_result();
}

void synchronouos_calculation_task::dispach_result()
{
   if(n_out_)
      dispach_synchronous();
   else {
      std::unique_lock<std::mutex> lock(end_mtx_);
      if(!kind_ & vertex_kind::command)
         resource_.stop();
      end_ = true;
      cond_.notify_one();
   }
}

void synchronouos_calculation_task::dispach_synchronous()
{
   vertex_calculation_task_map tasks = boost::get(boost::vertex_calculation_task, graph_);

   if(kind_ & vertex_kind::broadcast) {
      for(auto pos = out_begin_; pos != out_end_; ++pos) {
         datas_[*pos] = in_out_[0];
         auto v = boost::target(*pos, graph_);
         if(counters_[v]->notify_one(*pos))
            tasks[v]->move_to_ready();
      }
   }
   else {
      size_t i = 0;
      for(auto pos = out_begin_; pos != out_end_; ++pos, ++i) {
         datas_[*pos] = in_out_[i];
         auto v = boost::target(*pos, graph_);
         if(counters_[v]->notify_one(*pos))
            tasks[v]->move_to_ready();
      }
   }
}

} // namespace heterogeneous_graph
