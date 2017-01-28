// multi_call_counter.cpp
#include <iostream>
#include <mutex>
#include "multi_call_counter.hpp"
#include "resource.hpp"

namespace heterogeneous_graph {

multi_call_counter::multi_call_counter(
): _slots(0)
{
}

multi_call_counter::multi_call_counter(
   size_type n_inputs
): counter(n_inputs),
   _slots(0)
{
}

void multi_call_counter::init(
   size_type n_inputs
)
{
   _n_inputs = n_inputs;
}

bool multi_call_counter::notify_one(edge_descriptor idx)
{
   std::unique_lock<std::mutex> lock(_mutex);
   ++_counter;

   _slots.push_back(idx);
   if((static_cast<double>(_counter) / _n_inputs) > 0.1)
      _cond.notify_one();

   return is_ready();
}

bool multi_call_counter::available(std::vector<edge_descriptor> &slots)
{
   if(!_n_inputs)
      return false;

   {
      std::unique_lock<std::mutex> lock(_mutex);
      while(!_counter)
         _cond.wait(lock);

      _n_inputs -= _slots.size();
      _counter = 0;
      slots.swap(_slots);
   }
   return true;
}

} // namespace heterogeneous_graph
