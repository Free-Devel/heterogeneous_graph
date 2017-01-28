//types.hpp
#pragma once
#include <string>
#include <vector>
#include <functional>
#include <boost/any.hpp>

namespace heterogeneous_graph {

class environment;
class resource;

typedef boost::any data_type;
typedef std::vector<data_type> data_vector;
typedef std::function<data_vector (const data_vector &, environment &)> stuff_type;

} // namespace heterogeneous_graph
