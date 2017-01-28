// counter.cpp
#include <iostream>
#include <mutex>
#include "counter.hpp"
#include "resource.hpp"

namespace heterogeneous_graph {

counter::counter(
): _n_inputs(0),
   _counter(0)
{
}

counter::counter(
   size_type n_inputs
): _n_inputs(n_inputs),
   _counter(0)
{
}

counter::~counter()
{
}

void counter::init(
   size_type n_inputs
)
{
   _n_inputs = n_inputs;
}

bool counter::notify_one(edge_descriptor idx)
{
   ++_counter;

   return is_ready();
}

bool counter::is_ready() const
{
   return _counter == _n_inputs;
}

} // namespace heterogeneous_graph
