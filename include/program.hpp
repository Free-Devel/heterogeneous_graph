//program.hpp
#pragma once
#include <list>
#include <boost/shared_ptr.hpp>
#include "calculation_graph.hpp"

namespace heterogeneous_graph {

// Forward declaration.   
class command; 

struct program
{
   typedef std::list<calculation_graph> scope_list;
   
   program();
   calculation_graph &main_graph();
   
   scope_list scopes;
};
   
typedef program::scope_list::iterator scope;

} // namespace heterogeneous_graph

