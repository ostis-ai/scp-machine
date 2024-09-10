#pragma once

#include "scpOperator.hpp"

#include <sc-memory/sc_agent.hpp>

namespace scp
{

class ASCPMathOperatorInterpreter : public ScAgent<ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm>>
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event, ScAction & action)
      override;

  ScAddr GetEventSubscriptionElement() const override;

  bool CheckInitiationCondition(ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event) override;

  static inline ScKeynode const msAgentKeynode{"sc_agent_of_scp_operator_math_interpreting"};

  static void InitializeSupportedOperators();

private:
  static ScAddrToValueUnorderedMap<std::function<SCPOperator *(ScMemoryContext &, ScAddr)>> supportedOperators;
};

}  // namespace scp
