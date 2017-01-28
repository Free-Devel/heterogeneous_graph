//command.cpp
#include "commands/command.hpp"

namespace hg = heterogeneous_graph;

hg::command::command(program &prg): _scp(prg.scopes.begin()), _prg(prg)
{
}

void hg::command::set_scope(scope scp)
{
   _scp = scp;
}
