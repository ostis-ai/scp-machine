#pragma once

#include "scpOperatorInterpreter.hpp"

namespace scp
{

class ASCPStringOperatorInterpreter : public ASCPOperatorInterpreter
{
public:
  ScAddr GetActionClass() const override;

  ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
  getSupportedOperators() const override;

  static inline ScKeynode const msAgentKeynode{"sc_agent_of_scp_operator_string_interpreting"};

private:
  static ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
      supportedOperators;
};

}  // namespace scp
