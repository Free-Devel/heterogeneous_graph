////program_utility.cpp
//#include "program_utility.hpp"
//#include <boost/make_shared.hpp>
//#include "cpu_resource.hpp"
//#include "program.hpp"
//#include "utility_stuff.hpp"
//
//namespace heterogeneous_graph {
//namespace vk = vertex_kind;
//
//std::pair<vertex_descriptor, scope> add_node_0(
//   stuff_type stuff,
//   scope scp,
//   program &prg,
//   environment &env,
//   vertex_kind_type kind
//)
//{
//   calculation_graph &cg = *scp;
//   vertex_descriptor node = boost::add_vertex(cg);
//
//   init_node(node, stuff, cg, env, kind | vertex_kind::command);
//
//   return std::make_pair(node, scp);
//}
//
//std::pair<vertex_descriptor, scope> add_node_1(
//   stuff_type stuff,
//   scope scp,
//   program &prg,
//   vertex_descriptor pred,
//   environment &env,
//   vertex_kind_type kind
//)
//{
//   calculation_graph &cg = *scp;
//   vertex_descriptor node = boost::add_vertex(cg);
//   boost::add_edge(pred, node, cg);
//
//   init_node(node, stuff, cg, env, kind | vertex_kind::command);
//
//   return std::make_pair(node, scp);
//}
//
//std::pair<vertex_descriptor, scope> add_node_1(
//   _if cmd_if,
//   scope scp,
//   program &prg,
//   vertex_descriptor pred,
//   environment &env,
//   vertex_kind_type kind
//)
//{
//   calculation_graph &cg = *scp;
//
//   vertex_descriptor node = boost::add_vertex(cg);
//   boost::add_edge(pred, node, cg);
//   prg.scopes.push_back(heterogeneous_graph::calculation_graph());
//   cmd_if.set_scope(--prg.scopes.end());
//   init_node(node, cmd_if, cg, env, kind | vertex_kind::command);
//
//   return std::make_pair(node, --prg.scopes.end());
//}
//
//std::pair<vertex_descriptor, scope> add_node_1(
//   _for cmd_for,
//   scope scp,
//   program &prg,
//   vertex_descriptor pred,
//   environment &env,
//   vertex_kind_type kind
//)
//{
//   calculation_graph &cg = *scp;
//
//   vertex_descriptor node = boost::add_vertex(cg);
//   boost::add_edge(pred, node, cg);
//   prg.scopes.push_back(heterogeneous_graph::calculation_graph());
//   cmd_for.set_scope(--prg.scopes.end());
//   init_node(node, cmd_for, cg, env, kind | vertex_kind::command);
//
//   return std::make_pair(node, --prg.scopes.end());
//}
//
//std::pair<vertex_descriptor, scope> add_node_1(
//   _while cmd_while,
//   scope scp,
//   program &prg,
//   vertex_descriptor pred,
//   environment &env,
//   vertex_kind_type kind
//)
//{
//   calculation_graph &cg = *scp;
//
//   vertex_descriptor node = boost::add_vertex(cg);
//   boost::add_edge(pred, node, cg);
//   prg.scopes.push_back(heterogeneous_graph::calculation_graph());
//   cmd_while.set_scope(--prg.scopes.end());
//   init_node(node, cmd_while, cg, env, kind | vertex_kind::command);
//
//   return std::make_pair(node, --prg.scopes.end());
//}
//
//} // namespace heterogeneous_graph
