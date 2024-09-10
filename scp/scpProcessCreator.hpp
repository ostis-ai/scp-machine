/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>

namespace scp
{

class ASCPProcessCreator : public ScAgent<ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm>>
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

  ScAddr GetEventSubscriptionElement() const noexcept(false) override;

  bool CheckInitiationCondition(ScActionInitiatedEvent const & event) override;

  static inline ScKeynode const msAgentKeynode{"sc_agent_of_scp_process_creation"};
};

}  // namespace scp
