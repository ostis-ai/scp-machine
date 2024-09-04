/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpAgentProcessor.hpp"

#include "scpAgentEvent.hpp"
#include "scpKeynodes.hpp"

namespace scp {

ScResult ASCPAgentActivator::DoProgram(ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event,
                                       ScAction & action)
{
  ScAddr agent = event.GetOtherElement();

  SCPAgentEvent::register_scp_agent(m_context, agent);

  return action.FinishSuccessfully();
}

ScAddr ASCPAgentActivator::GetActionClass() const
{
  //todo(codegen-removal): replace action with your action class
  return ScKeynodes::action;
}

ScAddr ASCPAgentActivator::GetEventSubscriptionElement() const
{
  return Keynodes::active_sc_agent;
}

ScResult ASCPAgentDeactivator::DoProgram(ScEventBeforeEraseOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event,
                                         ScAction & action)
{
  ScAddr agent = event.GetOtherElement();
  SCPAgentEvent::unregister_scp_agent(m_context, agent);

  return action.FinishSuccessfully();
}

ScAddr ASCPAgentDeactivator::GetActionClass() const
{
  //todo(codegen-removal): replace action with your action class
  return ScKeynodes::action;
}

ScAddr ASCPAgentDeactivator::GetEventSubscriptionElement() const
{
  return Keynodes::active_sc_agent;
}

}
