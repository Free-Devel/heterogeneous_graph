// synchronouos_calculation_task.hpp
#pragma once
#include <string>
#include <memory>
#include <boost/config.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/thread/condition_variable.hpp>
#include "types.hpp"
#include "calculation_graph.hpp"
#include "calculation_task.hpp"
#include "counter.hpp"

namespace heterogeneous_graph {

class resource;

class synchronouos_calculation_task: public calculation_task
{
public:
   typedef synchronouos_calculation_task self_type;
   typedef self_type *ptr;
   typedef std::shared_ptr<self_type> shared_ptr;
   typedef std::vector<counter::shared_ptr> counter_vector_type;

   template <typename F>
   synchronouos_calculation_task(
      vertex_descriptor v,
      F &&stuff,
      calculation_graph &graph,
      environment &env,
      vertex_kind_type kind = vertex_kind::none
   ): calculation_task(std::move(stuff), env),
      vertex_(v),
      kind_(kind),
      graph_(graph),
      counters_(boost::get(boost::vertex_counter, graph)),
      datas_(boost::get(boost::edge_data, graph))
   {
   }

   template <typename F>
   synchronouos_calculation_task(
      boost::uuids::uuid uuid,
      vertex_descriptor v,
      F &&stuff,
      calculation_graph &graph,
      environment &env,
      vertex_kind_type kind = vertex_kind::none
   ): calculation_task(uuid, std::move(stuff), env),
      vertex_(v),
      kind_(kind),
      graph_(graph),
      counters_(boost::get(boost::vertex_counter, graph)),
      datas_(boost::get(boost::edge_data, graph))
   {
   }

   void init();

   bool is_ready() const;
   void run();

   void dispach_result();

private:
   vertex_descriptor vertex_;
   unsigned int kind_;
   calculation_graph &graph_;
   mutable vertex_counter_map counters_;
   edge_data_map datas_;

   size_t n_in_;
   in_edge_iterator in_begin_, in_end_;
   size_t n_out_;
   out_edge_iterator out_begin_, out_end_;

   void dispach_synchronous();
};

} // namespace heterogeneous_graph
