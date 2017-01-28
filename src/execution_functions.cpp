//execution_functions.cpp
#include <boost/make_shared.hpp>
#include "cpu_resource.hpp"
#include "execution_functions.hpp"
#include "utility_stuff.hpp"

namespace heterogeneous_graph {

data_vector run(const data_vector &input, calculation_graph &graph, environment &env)
{
   if(boost::num_vertices(graph) > 0) {
      vertex_descriptor root = boost::vertex(0, graph);
      vertex_descriptor end = boost::vertex(boost::num_vertices(graph) - 1, graph);

      vertex_calculation_task_map tasks = boost::get(boost::vertex_calculation_task, graph);
      vertex_iterator vi, vi_end;
      for(boost::tie(vi, vi_end) = vertices(graph); vi != vi_end; ++vi)
         tasks[*vi]->init();
 
      tasks[root]->set_input(input);
      tasks[root]->move_to_ready();

      return tasks[end]->get_output();
   }

   return data_vector();
}

} // namespace heterogeneous_graph
