// calculation_task_traits.hpp
#pragma once
#include "synchronouos_calculation_task.hpp"
#include "asynchronous_calculation_task.hpp"

namespace heterogeneous_graph {

template <typename KIND>
struct task_traits
{
   typedef void type;
};

template <>
struct task_traits<vertex_kind::synchronous>
{
   typedef synchronouos_calculation_task type;
};

template <>
struct task_traits<vertex_kind::asynchronous>
{
   typedef asynchronous_calculation_task type;
};

} // namespace heterogeneous_graph
