//cut_off_calculation_task.cpp
#include "cut_off_calculation_task.hpp"
#include "environment.hpp"

namespace heterogeneous_graph {

/*cut_off_calculation_task::cut_off_calculation_task(
   stuff_type stuff,
   environment &env
): calculation_task(stuff, env)
{
}*/

void cut_off_calculation_task::run()
{
   in_out_ = stuff_->run(in_out_, environment_);

   std::unique_lock<std::mutex> lock(end_mtx_);
   end_ = true;
   cond_.notify_one();
}

} // namespace heterogeneous_graph 
