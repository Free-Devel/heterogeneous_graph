//variable.cpp
#include "variable.hpp"

namespace heterogeneous_graph {

variable::variable()
{
}

variable::variable(const data_type &value): _value(value)
{
}

data_type variable::read() const
{
   return _value; 
}

void variable::write(const data_type &value)
{
   _value = value;   
}

} // namespace heterogeneous_graph
