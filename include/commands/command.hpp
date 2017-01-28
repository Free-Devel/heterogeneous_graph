//command.hpp
#pragma once
#include "types.hpp"
#include "program.hpp"

namespace heterogeneous_graph {

// Forward declarations.
class resource;
class environment;
   
class command
{
public:   
   command(program &prg);
   virtual data_vector operator()(const data_vector &data, environment &env) const = 0;
   
   void set_scope(scope scp);
protected:
   scope _scp;
   program &_prg;
};

} // namespace heterogeneous_graph
