//multi_call_multi_call_counter.hpp
#pragma once
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <boost/config.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/uuid/uuid.hpp>
#include "calculation_graph.hpp"
#include "counter.hpp"

namespace heterogeneous_graph {

class multi_call_counter: public counter
{
public:
   typedef size_t size_type;
   typedef std::shared_ptr<multi_call_counter> shared_ptr;

   multi_call_counter();
   multi_call_counter(
      size_type n_inputs
   );

   void init(
      size_type n_inputs
   );

   bool notify_one(edge_descriptor idx);
   bool available(std::vector<edge_descriptor> &slots);

private:
   std::vector<edge_descriptor> _slots;
   std::condition_variable _cond;
   std::mutex _mutex;
};

} // namespace heterogeneous_graph
