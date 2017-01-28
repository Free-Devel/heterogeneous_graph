//if.hpp
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
   
class _if: public command
{
public:   
   typedef std::function<bool (const data_vector &, environment &)> condition_type;
   typedef std::shared_ptr<_if> shared_ptr;

   _if(condition_type ct, program &prg);
   
   data_vector operator()(const data_vector &data, environment &env) const;
   
private:
   condition_type _condition;
};

} // namespace heterogeneous_graph
