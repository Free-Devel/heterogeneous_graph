//utility_stuff.hpp
#pragma once
#include <functional>
#include <algorithm>
#include <vector>
#include <boost/any.hpp>
#include "types.hpp"
#include "environment.hpp"
#include "calculation_graph.hpp"
//#include "connected_calculation_task.hpp"
#include "calculation_task_traits.hpp"
#include "compression_algorithms.hpp"
#include "commands/if.hpp"
#include "cpu_resource.hpp"

namespace heterogeneous_graph {

using std::placeholders::_1;

data_type start_stuff(const data_vector &data);
vertex_descriptor begin(calculation_graph &cg);
vertex_descriptor last(calculation_graph &cg);

template <typename KIND, typename F>
void init_node(
   vertex_descriptor vertex,
   F &&stuff,
   calculation_graph &graph,
   environment &env,
   vertex_kind_type kind = vertex_kind::none
)
{
   vertex_calculation_task_map tasks = boost::get(boost::vertex_calculation_task, graph);
   calculation_task::shared_ptr ct(new typename task_traits<KIND>::type(vertex, stuff, graph, env, kind));

   tasks[vertex] = ct;
   env.push(ct);
}

template <typename KINDS, typename KINDW, typename KINDR, typename FS, typename FW, typename FR>
void make_diamond_graph(
   FS &&sender,
   size_t n_workers,
   FW &&worker,
   FR &&receiver,
   calculation_graph &cg,
   environment &env,
   vertex_kind_type sender_kind = vertex_kind::none,
   vertex_kind_type worker_kind = vertex_kind::none,
   vertex_kind_type receiver_kind = vertex_kind::none
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

   init_node<KINDS>(
      start_node,
      sender,
      cg,
      env,
      sender_kind
   );

   for(size_t i = 0; i < n_workers; ++i) {
      init_node<KINDW>(
         start_node + i + 1,
         worker,
         cg,
         env,
         worker_kind
      );
   }

   init_node<KINDR>(
      end_node,
      receiver,
      cg,
      env,
      receiver_kind
   );
}

template <typename T>
data_vector marshall_vector(const std::vector<T> &data)
{
   data_vector out(data.size());
   std::transform(
      data.begin(),
      data.end(),
      out.begin(),
      [](auto elem) -> T { return elem; }
   );

   return out;
}

template <typename T>
data_type marshall_single(const T &d)
{
   return d;
}

template <typename T0>
data_vector marshall(const T0 &d0)
{
   data_vector out{ d0 };

   return out;
}

template <typename T0, typename T1>
data_vector marshall(const T0 &d0, const T1 &d1)
{
   data_vector out{ d0, d1 };

   return out;
}

template <typename T0, typename T1, typename T2>
data_vector marshall(const T0 &d0, const T1 &d1, const T2 &d2)
{
   data_vector out{ d0, d1, d2 };

   return out;
}

template <typename T>
T unmarshall(const data_type &data)
{
   T out = boost::any_cast<T>(data);

   return out;
}

template <typename T>
T unmarshall(const data_vector &data, size_t idx = 0)
{
   T out = boost::any_cast<T>(data[idx]);

   return out;
}

template <typename T>
std::vector<T> unmarshall_vector(const data_vector &data)
{
   std::vector<T> out(data.size());
   std::transform(
      data.begin(),
      data.end(),
      out.begin(),
      [](auto elem) { return boost::any_cast<T>(elem); }
   );

   return out;
}

} // namespace heterogeneous_graph
