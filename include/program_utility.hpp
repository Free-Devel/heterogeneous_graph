////program_utility.hpp
//#pragma once
//#include <boost/foreach.hpp>
//#include "calculation_graph.hpp"
//#include "program.hpp"
//#include "utility_stuff.hpp"
//#include "commands/if.hpp"
//#include "commands/for.hpp"
//#include "commands/while.hpp"
//
//namespace heterogeneous_graph {
//
//std::pair<vertex_descriptor, scope> add_node_0(
//   stuff_type stuff,
//   scope scp,
//   program &prg,
//   environment &env,
//   vertex_kind_type kind
//);
//
//std::pair<vertex_descriptor, scope> add_node_1(
//   stuff_type stuff,
//   scope scp,
//   program &prg,
//   vertex_descriptor pred,
//   environment &env,
//   vertex_kind_type kind
//);
//
//std::pair<vertex_descriptor, scope> add_node_1(
//   _if cmd_if,
//   scope scp,
//   program &prg,
//   vertex_descriptor pred,
//   environment &env,
//   vertex_kind_type kind
//);
//
//std::pair<vertex_descriptor, scope> add_node_1(
//   _for cmd_for,
//   scope scp,
//   program &prg,
//   vertex_descriptor pred,
//   environment &env,
//   vertex_kind_type kind
//);
//
//std::pair<vertex_descriptor, scope> add_node_1(
//   _while cmd_while,
//   scope scp,
//   program &prg,
//   vertex_descriptor pred,
//   environment &env,
//   vertex_kind_type kind
//);
//
//template<typename Range>
//std::pair<vertex_descriptor, scope> add_node_n(
//   stuff_type stuff,
//   scope scp,
//   program &prg,
//   const Range &preds,
//   environment &env,
//   vertex_kind_type kind
//)
//{
//   calculation_graph &cg = *scp;
//   vertex_descriptor node = boost::add_vertex(cg);
//
//   for(typename Range::const_iterator pred = preds.begin(); pred != preds.end(); ++pred)
//      boost::add_edge(*pred, node, cg);
//
//   init_node(node, stuff, cg, env, kind | vertex_kind::command);
//
//   return std::make_pair(node, scp);
//}
//
//} // namespace heterogeneous_graph
