//for.cpp
#include "commands/for.hpp"
#include "utility_stuff.hpp"
#include "execution_functions.hpp"

namespace hg = heterogeneous_graph;

hg::_for::_for(init_type init, condition_type cond, incr_type incr, program &prg): command(prg), _init(init), _condition(cond), _incr(incr)
{
}

hg::data_vector hg::_for::operator()(const data_vector &data, environment &env) const
{
   data_vector result = data_vector(1, hg::marshall_single<std::string>(""));
   data_vector input = data;

   for(_init(input, env); _condition(input, env); _incr(input, env)) 
      result = run(input, *_scp, env);
   
   std::cout << "_for::operator(): end." << std::endl;
   return result;
}
