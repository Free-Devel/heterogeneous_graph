//connected_calculation_task.hpp
//#pragma once
//#include <string>
//#include <memory>
//#include <boost/config.hpp>
//#include <boost/shared_ptr.hpp>
//#include <boost/uuid/uuid.hpp>
//#include <boost/thread/condition_variable.hpp>
//#include "types.hpp"
//#include "calculation_graph.hpp"
//#include "calculation_task.hpp"
//#include "counter.hpp"
//
//namespace heterogeneous_graph {
//
//class resource;
//
//template <typename KIND>
//class connected_calculation_task: 
//   public calculation_task,
//   public KIND
//{
//public:
//   typedef connected_calculation_task self_type;
//   typedef self_type *ptr;
//   typedef std::shared_ptr<self_type> shared_ptr;
//   typedef std::vector<counter::shared_ptr> counter_vector_type;
//   typedef KIND dispatch_type;
//
//   template <typename F>
//   connected_calculation_task(
//      vertex_descriptor v,
//      F &&stuff,
//      vertex_kind_type kind,
//      calculation_graph &graph,
//      environment &env
//   );
//
//   template <typename F>
//   connected_calculation_task(
//      boost::uuids::uuid uuid,
//      vertex_descriptor v,
//      F &&stuff,
//      vertex_kind_type kind,
//      calculation_graph &graph,
//      environment &env
//   );
//
//   void init();
//
//   bool is_ready() const;
//   void move_to_ready() const;
//   void run() { dispatch_type::run(this); }
//   
//   void dispach_result() { dispatch_type::dispach_result(this) }
//
//private:
//   vertex_descriptor vertex_;
//   unsigned int kind_;
//   calculation_graph &graph_;
//   mutable vertex_counter_map counters_;
//   edge_data_map datas_;
//
//   size_t n_in_;
//   in_edge_iterator in_begin_, in_end_;
//   size_t n_out_;
//   out_edge_iterator out_begin_, out_end_;
//
//   void dispach_synchronous();
//
//   void dispach_asynchronous_pre();
//   void dispach_asynchronous_post();
//
//   void dispach_auto_asynchronous();
//
//
//   friend class dispatch_type;
//};
//
//} // namespace heterogeneous_graph
