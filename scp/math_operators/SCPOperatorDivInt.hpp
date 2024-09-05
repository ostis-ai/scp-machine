#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"
#include <sc-memory/sc_agent.hpp>
#include "scpKeynodes.hpp"
#include "scpOperatorStr.hpp"

namespace scp
{

class SCPOperatorDivInt : public SCPOperatorElStr3
{
public:
  SCPOperatorDivInt(ScMemoryContext & ctx, ScAddr addr);
  std::string GetTypeName();
  sc_result Parse();
  sc_result Execute();
};
}  // namespace scp
