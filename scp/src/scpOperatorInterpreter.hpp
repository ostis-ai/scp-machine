/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>

#include "scpOperator.hpp"

namespace scp
{
class ASCPOperatorInterpreter : public ScAgent<ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc>>
{
public:
  ScResult DoProgram(ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc> const & event, ScAction & action)
      override;

  ScAddr GetEventSubscriptionElement() const override;

  virtual ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
  getSupportedOperators() const = 0;

  bool CheckInitiationCondition(ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc> const & event) override;

private:
};

}  // namespace scp
