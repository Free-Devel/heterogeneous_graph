//counter.hpp
#pragma once
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <boost/config.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/uuid/uuid.hpp>
#include "calculation_graph.hpp"

namespace heterogeneous_graph {

class counter
{
public:
   typedef size_t size_type;
   typedef std::shared_ptr<counter> shared_ptr;

   counter();
   counter(
      size_type n_inputs
   );
   virtual ~counter();

   virtual void init(
      size_type n_inputs
   );
   
   bool is_ready() const;
   virtual bool notify_one(edge_descriptor idx);

protected:
   size_type _n_inputs;
   std::atomic<size_type> _counter;
};

} // namespace heterogeneous_graph
