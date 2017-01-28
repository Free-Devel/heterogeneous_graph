// layer.hpp
#pragma once
#include <vector>
#include "types.hpp"
#include "calculation_graph.hpp"

namespace heterogeneous_graph {

class layer
{
public:
   layer(
      stuff_type stuff,
      vertex_kind_type kind
   );

   layer(
      size_t n,
      stuff_type stuff,
      vertex_kind_type kind
   );

   template<typename It>
   layer(
      const std::pair<It, It> &stuff_range, 
      vertex_kind_type kind
   ): data_{ std::vector<stuff_type>(stuff_range.first, stuff_range.second), kind },
      succ_(nullptr),
      paral_(nullptr)
   {
   }

   calculation_graph operator()() const;

private:
   struct {
      std::vector<stuff_type> stuffs_;
      vertex_kind_type kind_;
   } data_;

   const layer *succ_;
   const layer *paral_;

   friend layer &operator-(layer &, const layer &);
   friend layer &operator|(layer &, const layer &);
};

layer &operator-(layer &lhs, const layer &rhs);
layer &operator|(layer &lhs, const layer &rhs);

} // namespace heterogeneous_graph
