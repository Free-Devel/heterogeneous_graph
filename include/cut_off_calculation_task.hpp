//cut_off_calculation_task.hpp
#pragma once
#include <memory>
#include <string>
#include <boost/config.hpp>
#include "types.hpp"
#include "calculation_task.hpp"

namespace heterogeneous_graph {

class resource;
class environment;

class cut_off_calculation_task: public calculation_task
{
public:
   typedef cut_off_calculation_task self_type;
   typedef self_type *ptr;
   typedef std::shared_ptr<self_type> shared_ptr;

   template <typename F>
   cut_off_calculation_task(
      F &&stuff,
      environment &env
   ): calculation_task(std::move(stuff), env)
   {
   }

   void init() {}

   void run();

private:

};

} // namespace heterogeneous_graph
