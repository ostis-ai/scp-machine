/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpAgentProcessor.hpp"

#include "scpAgentEvent.hpp"
#include "scpKeynodes.hpp"

namespace scp
{

ScResult ASCPAgentActivator::DoProgram(
    ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc> const & event,
    ScAction & action)
{
  ScAddr agent = event.GetOtherElement();

  SCPAgentEvent::HandleActiveAgent(m_context, SCPAgentEvent::RegisterSCPAgent, agent);

  return action.FinishSuccessfully();
}

ScAddr ASCPAgentActivator::GetActionClass() const
{
  return Keynodes::action_activate_agent;
}

ScAddr ASCPAgentActivator::GetEventSubscriptionElement() const
{
  return Keynodes::active_sc_agent;
}

ScResult ASCPAgentDeactivator::DoProgram(
    ScEventBeforeEraseOutgoingArc<ScType::ConstPermPosArc> const & event,
    ScAction & action)
{
  ScAddr agent = event.GetOtherElement();

  SCPAgentEvent::HandleActiveAgent(m_context, SCPAgentEvent::UnregisterSCPAgent, agent);

  return action.FinishSuccessfully();
}

ScAddr ASCPAgentDeactivator::GetActionClass() const
{
  return Keynodes::action_deactivate_agent;
}

ScAddr ASCPAgentDeactivator::GetEventSubscriptionElement() const
{
  return Keynodes::active_sc_agent;
}

}  // namespace scp
