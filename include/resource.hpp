//resource.hpp
#pragma once
#include <memory>
#include <boost/config.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/utility.hpp>

namespace heterogeneous_graph {

class calculation_task;

class resource: private boost::noncopyable
{
public:
   typedef std::shared_ptr<resource> shared_ptr;
   typedef resource *pointer;

   resource(const boost::uuids::uuid &u);
   virtual ~resource();

   virtual void move_to_ready(calculation_task *) = 0;
   virtual void stop() = 0;

   boost::uuids::uuid uuid() const;

protected:
   boost::uuids::uuid _uuid;
};

} // namespace heterogeneous_graph
