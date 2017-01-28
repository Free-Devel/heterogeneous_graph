//resource.cpp
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "resource.hpp"
#include "calculation_task.hpp"

namespace heterogeneous_graph {

resource::resource(const boost::uuids::uuid &u): _uuid(u)
{
}

resource::~resource()
{
}

boost::uuids::uuid resource::uuid() const
{
   return _uuid;
}

} // namespace heterogeneous_graph
