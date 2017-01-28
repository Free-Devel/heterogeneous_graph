//dynamic_stuff_node.cpp
#include <boost/ptr_container/ptr_vector.hpp>
#include "dynamic_stuff_node.hpp"
#include "calculation_task.hpp"
#include "cut_off_calculation_task.hpp"
#include "resource.hpp"

namespace heterogeneous_graph {

dynamic_stuff_node::dynamic_stuff_node(
   splitting_type splitting_agent,
   merging_type merging_agent,
   stuff_type stuff
): _splitting_agent(splitting_agent),
   _merging_agent(merging_agent),
   _stuff(stuff)
{
}

dynamic_stuff_node::~dynamic_stuff_node()
{
}

data_vector dynamic_stuff_node::operator()(const data_vector &data, environment &env) const
{
   data_vector loc_data = _splitting_agent(data);
   size_t multiplicity = loc_data.size();

   data_vector output(multiplicity);

   boost::ptr_vector<calculation_task> tasks;
   for(size_t i = 0; i < multiplicity - 1; ++i) {
      tasks.push_back(new cut_off_calculation_task(_stuff, env));
      tasks[i].set_input(data_vector(1, loc_data[i]));
      env.get_main_resource().move_to_ready(&tasks[i]);
   }
   output[multiplicity - 1] = _stuff(data_vector(1, loc_data[multiplicity - 1]), env)[0];

   for(size_t i = 0; i < multiplicity - 1; ++i) 
      output[i] = tasks[i].get_output()[0];

   return _merging_agent(output);
}

} //namespace heterogeneous_graph 
