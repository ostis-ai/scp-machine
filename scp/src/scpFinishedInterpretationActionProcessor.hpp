/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>

namespace scp
{

class ASCPFinishedInterpretationActionProcessor
  : public ScAgent<ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc>>
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc> const & event, ScAction & action)
      override;

  ScAddr GetEventSubscriptionElement() const override;

  bool CheckInitiationCondition(ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc> const & event) override;

  static inline ScKeynode const msAgentKeynode{"sc_agent_of_finished_scp_interpretation_action_processing"};
};

}  // namespace scp
