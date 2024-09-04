/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpAgentProcessor.hpp"
#include "sc-memory/sc_memory.hpp"

namespace scp {

ScAddr ASCPAgentActivator::msAgentKeynode;
ScAddr ASCPAgentDeactivator::msAgentKeynode;

ScResult ASCPAgentActivator::DoProgram(ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event, ScAction & action)
{
    ScAddr agent =m_context.GetEdgeTarget(event.GetArc());

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

SC_AGENT_IMPLEMENTATION(ASCPAgentDeactivator)
{
    ScAddr agent = otherAddr;
    SCPAgentEvent::unregister_scp_agent(m_context, agent);

    return SC_RESULT_OK;
}

}
