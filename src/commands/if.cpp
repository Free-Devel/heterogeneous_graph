//if.cpp
#include "commands/if.hpp"
#include "utility_stuff.hpp"
#include "execution_functions.hpp"

namespace hg = heterogeneous_graph;

hg::_if::_if(condition_type ct, program &prg): command(prg), _condition(ct)
{
}

hg::data_vector hg::_if::operator()(const data_vector &data, environment &env) const
{
   if(_condition(data, env))
      return run(data, *_scp, env);
   
   return data_vector(1, hg::marshall_single<std::string>(""));
}
