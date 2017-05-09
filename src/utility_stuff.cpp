//utility_stuff.cpp
#include <boost/shared_array.hpp>
#include <boost/make_shared.hpp>
#include "types.hpp"
#include "utility_stuff.hpp"
#include "cpu_resource.hpp"
#include "exception.hpp"

namespace heterogeneous_graph {
namespace vk = vertex_kind;

void connect(size_t a, size_t b)
{

}


/*void init_node(
   vertex_descriptor vertex,
   stuff_type stuff,
   calculation_graph &graph,
   environment &env,
   vertex_kind_type kind
)
{
   vertex_calculation_task_map tasks = boost::get(boost::vertex_calculation_task, graph);
   connected_calculation_task::shared_ptr ct(new connected_calculation_task(vertex, stuff, kind, graph, env));

   tasks[vertex] = ct;
   env.push(ct);
}*/

/*void make_diamond_graph(
   stuff_type sender,
   size_t n_workers,
   stuff_type worker,
   stuff_type receiver,
   calculation_graph &cg,
   environment &env,
   vertex_kind_type sender_kind,
   vertex_kind_type worker_kind,
   vertex_kind_type receiver_kind
)
{
   typedef std::pair<size_t, size_t> link_type;

   for(size_t i = 0; i < n_workers + 2; ++i)
      boost::add_vertex(cg);

   std::vector<link_type> edges(n_workers * 2);
   for(size_t i = 0; i < n_workers; ++i)
      boost::add_edge(0, i + 1, cg);
   for(size_t i = 0; i < n_workers; ++i)
      boost::add_edge(i + 1, n_workers + 1, cg);

   vertex_descriptor start_node = 0;
   vertex_descriptor end_node = n_workers + 1;

   init_node(
      start_node,
      sender,
      cg,
      env,
      sender_kind
      );

   for(size_t i = 0; i < n_workers; ++i) {
      init_node(
         start_node + i + 1,
         worker,
         cg,
         env,
         worker_kind
         );
   }

   init_node(
      end_node,
      receiver,
      cg,
      env,
      receiver_kind
      );
}*/

data_type start_stuff(const data_vector &data)
{
   return data[0];
}

vertex_descriptor begin(calculation_graph &cg)
{
   return *(boost::vertices(cg).first);
}

vertex_descriptor last(calculation_graph &cg)
{
   return *(--(boost::vertices(cg).second));
}

timer::timer(
): start(std::chrono::high_resolution_clock::now()),
   ended(false)
{
}

void timer::stop()
{
   end = std::chrono::high_resolution_clock::now();
   ended = true;
}

std::chrono::duration<double> timer::elapsed()
{
   if(!ended)
      stop();

   return end - start;
}

} // namespace heterogeneous_graph
