//execution_funtions.hpp
#pragma once
#include <vector>
#include <boost/config.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/utility.hpp>
#include <boost/make_shared.hpp>
#include <boost/timer/timer.hpp>
#include "types.hpp"
#include "calculation_graph.hpp"
#include "connected_calculation_task.hpp"
#include "environment.hpp"
#include "cpu_resource.hpp"
#include "utility_stuff.hpp"

namespace heterogeneous_graph {

template <typename Input, typename Result>
Result run(const Input &input, calculation_graph &graph, environment &env)
{
   if(boost::num_vertices(graph) > 0) {
      vertex_descriptor root = boost::vertex(0, graph);
      vertex_descriptor end = boost::vertex(boost::num_vertices(graph) - 1, graph);

      vertex_calculation_task_map tasks = boost::get(boost::vertex_calculation_task, graph);
      vertex_iterator vi, vi_end;
      for(boost::tie(vi, vi_end) = vertices(graph); vi != vi_end; ++vi)
         tasks[*vi]->init();

      tasks[root]->set_input(marshall(input));
      tasks[root]->move_to_ready();

      return unmarshall<Result>(tasks[end]->get_output());
   }

   return Result();
}

data_vector run(const data_vector &input, calculation_graph &graph, environment &env);

} // namespace heterogeneous_graph
