//exception.hpp
#pragma once
#include <exception>
#include <string>

namespace heterogeneous_graph {

class hg_exception: public std::exception
{
public:
   hg_exception(const std::string &msg);
   virtual ~hg_exception() throw();
   const char* what() const throw();

private:
   std::string _msg;
};

} // namespace heterogeneous_graph
