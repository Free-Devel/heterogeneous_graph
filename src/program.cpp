//program.cpp
#include "program.hpp"

namespace heterogeneous_graph {

program::program(): scopes(1)
{
}

calculation_graph &program::main_graph()
{
   return scopes.front();
}

} // namespace heterogeneous_graph
