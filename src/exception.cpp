//exception.cpp
#include "exception.hpp"

namespace heterogeneous_graph {

hg_exception::hg_exception(const std::string &msg): _msg(msg)
{
}

hg_exception::~hg_exception() throw()
{
}

const char* hg_exception::what() const throw()
{
   return _msg.c_str();
}

} // namespace heterogeneous_graph
