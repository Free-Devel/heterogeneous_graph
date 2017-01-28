//connected_calculation_task.cpp
//#include <mutex>
//#include <boost/config.hpp>
//#include <boost/lexical_cast.hpp>
//#include <boost/uuid/uuid_io.hpp>
//#include <boost/uuid/random_generator.hpp>
//#include <boost/any.hpp>
//#include "connected_calculation_task.hpp"
//#include "utility_stuff.hpp"
//#include "counter.hpp"
//#include "multi_call_counter.hpp"
//
//namespace heterogeneous_graph {
//namespace vk = vertex_kind;
//
//connected_calculation_task::connected_calculation_task(
//   vertex_descriptor v,
//   stuff_type stuff,
//   vertex_kind_type kind,
//   calculation_graph &graph,
//   environment &env
//): calculation_task(stuff, env),
//   vertex_(v),
//   kind_(kind),
//   graph_(graph),
//   counters_(boost::get(boost::vertex_counter, graph)),
//   datas_(boost::get(boost::edge_data, graph))
//{
//}
//
//connected_calculation_task::connected_calculation_task(
//      boost::uuids::uuid uuid,
//      vertex_descriptor v,
//      stuff_type stuff,
//      vertex_kind_type kind,
//      calculation_graph &graph,
//      environment &env
//): calculation_task(uuid, stuff, env),
//   vertex_(v),
//   kind_(kind),
//   graph_(graph),
//   counters_(boost::get(boost::vertex_counter, graph)),
//   datas_(boost::get(boost::edge_data, graph))
//{
//}
//
//void connected_calculation_task::init()
//{
//   end_ = false;
//
//   n_in_ = boost::in_degree(vertex_, graph_);
//   boost::tie(in_begin_, in_end_) = boost::in_edges(vertex_, graph_);
//   if((kind_ & vk::multi_call) != 0)
//      counters_[vertex_].reset(new multi_call_counter(n_in_));
//   else
//      counters_[vertex_].reset(new counter(n_in_));
//
//   n_out_ = boost::out_degree(vertex_, graph_);
//   boost::tie(out_begin_, out_end_) = boost::out_edges(vertex_, graph_);
//}
//
//bool connected_calculation_task::is_ready() const
//{
//   return counters_[vertex_]->is_ready();
//}
//
//void connected_calculation_task::move_to_ready() const
//{
//   _resource.move_to_ready(const_cast<connected_calculation_task *>(this));
//}
//
//void connected_calculation_task::run()
//{
//   if(kind_ & vertex_kind::multi_call) {
//      vertex_counter_map counters;
//
//      in_out_.resize(n_in_);
//      std::vector<edge_descriptor> slots;
//      while(counters_[vertex_]->available(slots)) {
//         data_vector input(slots.size());
//         for(size_t i = 0; i < slots.size(); ++i)
//            in_out_[i] = datas_[slots[i]];
//
//         in_out_ = stuff_(in_out_, _environment);
//         slots.clear();
//      }
//   }
//   else {
//      if(n_in_) {
//         in_out_.resize(n_in_);
//         size_t i = 0;
//         for(in_edge_iterator pos = in_begin_; pos != in_end_; ++pos, ++i)
//            in_out_[i] = datas_[*pos];
//      }
//
//      if(kind_ & vertex_kind::asynchronous)
//         dispach_asynchronous_pre();
//      else if(kind_ & vertex_kind::auto_asynchronous)
//         dispach_auto_asynchronous();
//
//      in_out_ = stuff_(in_out_, _environment);
//   }
//
//   if(!(kind_ & vertex_kind::asynchronous_resource))
//      dispach_result();
//}
//
//void connected_calculation_task::dispach_result()
//{
//   if(n_out_) {
//      if(kind_ & vertex_kind::synchronous)
//         dispach_synchronous();
//      else if(kind_ & vertex_kind::asynchronous)
//         dispach_asynchronous_post();
//   }
//   else {
//      std::unique_lock<std::mutex> lock(end_mtx_);
//       if(!kind_ & vertex_kind::command)
//         _resource.stop();
//      end_ = true;
//      cond_.notify_one();
//   }
//}
//
//void connected_calculation_task::dispach_synchronous()
//{
//   vertex_calculation_task_map tasks = boost::get(boost::vertex_calculation_task, graph_);
//
//   if(kind_ & vertex_kind::broadcast) {
//      for(out_edge_iterator pos = out_begin_; pos != out_end_; ++pos) {
//         datas_[*pos] = in_out_[0];
//         vertex_descriptor v = boost::target(*pos, graph_);
//         if(counters_[v]->notify_one(*pos))
//            tasks[v]->move_to_ready();
//      }
//   }
//   else {
//      size_t i = 0;
//      for(out_edge_iterator pos = out_begin_; pos != out_end_; ++pos, ++i) {
//         datas_[*pos] = in_out_[i];
//         vertex_descriptor v = boost::target(*pos, graph_);
//         if(counters_[v]->notify_one(*pos))
//            tasks[v]->move_to_ready();
//      }
//   }
//}
//
//void connected_calculation_task::dispach_asynchronous_pre()
//{
//   vertex_calculation_task_map tasks = boost::get(boost::vertex_calculation_task, graph_);
//
//   for(out_edge_iterator pos = out_begin_; pos != out_end_; ++pos) {
//      boost::uuids::uuid uuid = _environment.new_promise_future();
//      datas_[*pos] = uuid;
//   }
//   for(out_edge_iterator pos = out_begin_; pos != out_end_; ++pos) {
//      vertex_descriptor v = boost::target(*pos, graph_);
//      if(counters_[v]->notify_one(*pos))
//         tasks[v]->move_to_ready();
//   }
//}
//
//void connected_calculation_task::dispach_asynchronous_post()
//{
//   size_t i = 0;
//   for(out_edge_iterator pos = out_begin_; pos != out_end_; ++pos, ++i) {
//      boost::uuids::uuid uuid = boost::any_cast<boost::uuids::uuid>(datas_[*pos])/*boost::lexical_cast<boost::uuids::uuid>(_datas[*pos])*/;
//      _environment.set_promise(in_out_[i], uuid);
//   }
//}
//
//void connected_calculation_task::dispach_auto_asynchronous()
//{
//   vertex_calculation_task_map tasks = boost::get(boost::vertex_calculation_task, graph_);
//
//   for(out_edge_iterator pos = out_begin_; pos != out_end_; ++pos) {
//      boost::uuids::uuid uuid = _environment.new_promise_future();
//      std::string us = boost::uuids::to_string(uuid);
//
//      in_out_.push_back(us);
//      datas_[*pos] = us;
//      vertex_descriptor v = boost::target(*pos, graph_);
//      if(counters_[v]->notify_one(*pos))
//         tasks[v]->move_to_ready();
//   }
//}
//
//} // namespace heterogeneous_graph
