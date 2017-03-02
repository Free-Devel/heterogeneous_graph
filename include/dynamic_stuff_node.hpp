//dynamic_stuff_node.hpp
#pragma once
#include <memory>
#include <functional>
#include "types.hpp"
#include "environment.hpp"

namespace heterogeneous_graph {

class dynamic_stuff_node
{
public:
   typedef dynamic_stuff_node self_type;
   typedef self_type *pointer;
   typedef std::shared_ptr<self_type> shared_ptr;
   //typedef stuff_type stuff_type;

   typedef std::function<data_vector (const data_vector &)> splitting_type;
   typedef std::function<data_vector (const data_vector &)> merging_type;
   
   dynamic_stuff_node(
      splitting_type splitting_agent,
      merging_type merging_agent,
      stuff_type stuff
   );
   virtual ~dynamic_stuff_node();

   data_vector operator()(const data_vector &data, environment &env) const;

private:
   splitting_type _splitting_agent;
   merging_type _merging_agent;
   stuff_type _stuff;
};

} // namespace heterogeneous_graph

