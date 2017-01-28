// layer.cpp
#include "layer.hpp"

namespace heterogeneous_graph {

layer::layer(
   stuff_type stuff,
   vertex_kind_type kind
): data_{ std::vector<stuff_type>{ stuff }, kind },
   succ_(nullptr),
   paral_(nullptr)
{
}

layer::layer(
   size_t n,
   stuff_type stuff,
   vertex_kind_type kind
): data_{ std::vector<stuff_type>(n, stuff), kind },
   succ_(nullptr),
   paral_(nullptr)
{
}

calculation_graph layer::operator()() const
{
   return calculation_graph();
}

layer &operator-(layer &lhs, const layer &rhs)
{
   lhs.succ_ = &rhs;
   return lhs;
}

layer &operator|(layer &lhs, const layer &rhs)
{
   lhs.paral_ = &rhs;
   return lhs;
}

} // namespace heterogeneous_graph
