// asynchronous_calculation_task.cpp
#include <mutex>
#include <boost/config.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/any.hpp>
#include "asynchronous_calculation_task.hpp"
#include "utility_stuff.hpp"
#include "counter.hpp"
#include "multi_call_counter.hpp"

namespace heterogeneous_graph {
namespace vk = vertex_kind;

void asynchronous_calculation_task::init()
{
   end_ = false;

   n_in_ = boost::in_degree(vertex_, graph_);
   boost::tie(in_begin_, in_end_) = boost::in_edges(vertex_, graph_);
   counters_[vertex_].reset(new counter(n_in_));

   n_out_ = boost::out_degree(vertex_, graph_);
   boost::tie(out_begin_, out_end_) = boost::out_edges(vertex_, graph_);
}

bool asynchronous_calculation_task::is_ready() const
{
   return counters_[vertex_]->is_ready();
}

void asynchronous_calculation_task::run()
{
   if(n_in_) {
      in_out_.resize(n_in_);
      size_t i = 0;
      for(auto pos = in_begin_; pos != in_end_; ++pos, ++i)
         in_out_[i] = datas_[*pos];
   }
   
   dispach_asynchronous_pre();
   
   in_out_ = stuff_->run(in_out_, environment_);

   dispach_result();
}

void asynchronous_calculation_task::dispach_result()
{
   if(n_out_)
      dispach_asynchronous_post();
   else {
      std::unique_lock<std::mutex> lock(end_mtx_);
      if(!kind_ & vertex_kind::command)
         resource_.stop();
      end_ = true;
      cond_.notify_one();
   }
}

void asynchronous_calculation_task::dispach_asynchronous_pre()
{
   vertex_calculation_task_map tasks = boost::get(boost::vertex_calculation_task, graph_);

   if(kind_ & vertex_kind::broadcast) {
      boost::uuids::uuid uuid = environment_.new_promise_future();

      for(auto pos = out_begin_; pos != out_end_; ++pos) {
         datas_[*pos] = uuid;
         auto v = boost::target(*pos, graph_);
         if(counters_[v]->notify_one(*pos))
            tasks[v]->move_to_ready();
      }
   }
   else {
      for(auto pos = out_begin_; pos != out_end_; ++pos) {
         boost::uuids::uuid uuid = environment_.new_promise_future();
         datas_[*pos] = uuid;
      }
      for(auto pos = out_begin_; pos != out_end_; ++pos) {
         auto v = boost::target(*pos, graph_);
         if(counters_[v]->notify_one(*pos))
            tasks[v]->move_to_ready();
      }
   }
}

void asynchronous_calculation_task::dispach_asynchronous_post()
{
   if(kind_ & vertex_kind::broadcast) {
      auto uuid = boost::any_cast<boost::uuids::uuid>(datas_[*out_begin_]);
      environment_.set_promise(in_out_[0], uuid);
   }
   else {
      size_t i = 0;
      for(auto pos = out_begin_; pos != out_end_; ++pos, ++i) {
         auto uuid = boost::any_cast<boost::uuids::uuid>(datas_[*pos]);
         environment_.set_promise(in_out_[i], uuid);
      }
   }
}

} // namespace heterogeneous_graph
