/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "scpOperator.hpp"

#include <sc-memory/sc_agent.hpp>

namespace scp
{

class ASCPGenOperatorInterpreter : public ScAgent<ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm>>
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event, ScAction & action)
      override;

  ScAddr GetEventSubscriptionElement() const override;

  bool CheckInitiationCondition(ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event) override;

  static inline ScKeynode const msAgentKeynode{"sc_agent_of_scp_operator_gen_interpreting"};

private:
  static ScAddrToValueUnorderedMap<std::function<SCPOperator *(ScMemoryContext &, ScAddr)>> supportedOperators;
};

}  // namespace scp
