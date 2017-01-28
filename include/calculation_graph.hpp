//calculation_graph.hpp
#pragma once
#include <memory>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include "types.hpp"
#include "calculation_graph_property.hpp"

namespace heterogeneous_graph {

// forward declarations
class calculation_task;
class counter;
class resource;
class environment;

// vertex properties
typedef
   boost::property<boost::vertex_calculation_task_t, std::shared_ptr<calculation_task>,
      boost::property<boost::vertex_counter_t, std::shared_ptr<counter>>> vertex_property_type;

// edge properties
typedef
   boost::property<boost::edge_data_t, data_type> edge_property_type;

// graph definition
typedef
   boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
      vertex_property_type, edge_property_type> calculation_graph;

typedef const unsigned int vertex_kind_type;

namespace vertex_kind {
extern vertex_kind_type none;
extern vertex_kind_type broadcast;

struct synchronous {};
struct asynchronous {};
struct auto_asynchronous {};
struct multi_call {};
//struct asynchronous_resource {};

extern vertex_kind_type cmd_if;
extern vertex_kind_type cmd_for;
extern vertex_kind_type cmd_while;
extern vertex_kind_type command;
}

typedef boost::graph_traits<calculation_graph>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<calculation_graph>::vertex_iterator vertex_iterator;
typedef boost::graph_traits<calculation_graph>::edge_descriptor edge_descriptor;
typedef boost::graph_traits<calculation_graph>::edge_iterator edge_iterator;

typedef boost::graph_traits<calculation_graph>::in_edge_iterator in_edge_iterator;
typedef boost::graph_traits<calculation_graph>::out_edge_iterator out_edge_iterator;

typedef boost::property_map<calculation_graph, boost::vertex_calculation_task_t>::type vertex_calculation_task_map;
typedef boost::property_map<calculation_graph, boost::vertex_calculation_task_t>::const_type const_vertex_calculation_task_map;

typedef boost::property_map<calculation_graph, boost::vertex_counter_t>::type vertex_counter_map;
typedef boost::property_map<calculation_graph, boost::vertex_counter_t>::const_type const_vertex_counter_map;

typedef boost::property_map<calculation_graph, boost::edge_data_t>::type edge_data_map;
typedef boost::property_map<calculation_graph, boost::edge_data_t>::const_type const_edge_data_map;

} // namespace heterogeneous_graph

/*
todo:
- gestione cache environment/resource.
- monitor per resource.
- risorsa CUDA.
*/

