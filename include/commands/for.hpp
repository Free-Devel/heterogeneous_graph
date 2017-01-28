//for.hpp
#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "types.hpp"
#include "program.hpp"
#include "command.hpp"

namespace heterogeneous_graph {

class resource;
class environment;
   
class _for: public command
{
public:   
   typedef std::function<void (const data_vector &, environment &)> init_type;
   typedef std::function<bool (const data_vector &, environment &)> condition_type;
   typedef std::function<void (const data_vector &, environment &)> incr_type;
   typedef std::shared_ptr<_for> shared_ptr;

   _for(init_type init, condition_type cond, incr_type incr, program &prg);
   
   data_vector operator()(const data_vector &data, environment &env) const;
   
private:
   init_type _init;
   condition_type _condition;
   incr_type _incr;
};

} // namespace heterogeneous_graph
