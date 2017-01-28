//calculation_graph.cpp
#include "calculation_graph.hpp"

namespace heterogeneous_graph { namespace vertex_kind {

const unsigned int none                  = 0x00000000;
const unsigned int broadcast             = 0x00000001;
//const unsigned int synchronous           = 0x00000002;
//const unsigned int asynchronous          = 0x00000004;
//const unsigned int auto_asynchronous     = 0x00000008;
//const unsigned int multi_call            = 0x00000010;
//const unsigned int asynchronous_resource = 0x00000020;

const unsigned int cmd_if                = 0x00008400;
const unsigned int cmd_for               = 0x00009000;
const unsigned int cmd_while             = 0x0000C000;
const unsigned int command               = 0x00008000;

} } // namespace heterogeneous_graph vertex_kind
