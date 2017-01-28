//while.cpp
#include "commands/while.hpp"
#include "utility_stuff.hpp"
#include "execution_functions.hpp"

namespace hg = heterogeneous_graph;

hg::_while::_while(condition_type ct, program &prg): command(prg), _condition(ct)
{
}

hg::data_vector hg::_while::operator()(const data_vector &data, environment &env) const
{
   data_vector input = data;
   while(_condition(input, env)) 
      input = run(input, *_scp, env);
   
   return input;
}
