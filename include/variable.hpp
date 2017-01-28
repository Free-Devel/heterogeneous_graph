//variable.hpp
#pragma once
#include <string>
#include <boost/config.hpp>
#include <boost/shared_ptr.hpp>
#include "types.hpp"

namespace heterogeneous_graph {

class variable
{
public:
   variable();
   variable(const data_type &value);
   
   data_type read() const;
   void write(const data_type &value);   
   
private:   
   data_type _value;
};

} // namespace heterogeneous_graph
