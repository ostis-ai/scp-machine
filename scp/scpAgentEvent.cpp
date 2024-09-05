/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpAgentEvent.hpp"

#include "scp.hpp"
#include "scpInterpretationRequestInitiationAgent.hpp"
#include "scpKeynodes.hpp"

namespace scp
{
void SCPAgentEvent::register_all_scp_agents(ScAgentContext & ctx)
{
  auto const & activeAgentsIterator =
      ctx.Iterator3(Keynodes::active_sc_agent, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
  while (activeAgentsIterator->Next())
  {
    ScAddr const & agent = activeAgentsIterator->Get(2);
    auto const & agentImplementationsIterator = ctx.Iterator3(ScType::NodeConst, ScType::EdgeAccessConstPosPerm, agent);
    while (agentImplementationsIterator->Next())
    {
      ScAddr const & agentImplementation = agentImplementationsIterator->Get(0);
      if (agentImplementation == Keynodes::active_sc_agent)
        continue;
      if (ctx.HelperCheckEdge(
              Keynodes::platform_independent_abstract_sc_agent, agentImplementation, ScType::EdgeAccessConstPosPerm))
      {
        SCP_LOG_DEBUG("found independent abstract sc agent for agent " << ctx.HelperGetSystemIdtf(agent));
        register_scp_agent(ctx, agentImplementation);
      }
    }
  }
}

void SCPAgentEvent::unregister_all_scp_agents(ScAgentContext & ctx)
{
  auto const & activeAgentsIterator =
      ctx.Iterator3(Keynodes::active_sc_agent, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
  while (activeAgentsIterator->Next())
  {
    ScAddr const & agent = activeAgentsIterator->Get(2);
    auto const & agentImplementationsIterator = ctx.Iterator3(ScType::NodeConst, ScType::EdgeAccessConstPosPerm, agent);
    while (agentImplementationsIterator->Next())
    {
      ScAddr const & agentImplementation = agentImplementationsIterator->Get(0);
      if (ctx.HelperCheckEdge(
              Keynodes::platform_independent_abstract_sc_agent, agentImplementation, ScType::EdgeAccessConstPosPerm))
        unregister_scp_agent(ctx, agentImplementation);
    }
  }
}

void SCPAgentEvent::register_scp_agent(ScAgentContext & ctx, ScAddr const & agentNode)
{
  try
  {
    ctx.SubscribeSpecifiedAgent<SCPInterpretationRequestInitiationAgent>(agentNode);
  }
  catch (utils::ScException const & exception)
  {
    SCP_LOG_ERROR("Cannot register agent " << agentNode.Hash() << "\n" << exception.Description());
  }
}

void SCPAgentEvent::unregister_scp_agent(ScAgentContext & ctx, ScAddr const & agentNode)
{
  try
  {
    ctx.UnsubscribeSpecifiedAgent<SCPInterpretationRequestInitiationAgent>(agentNode);
  }
  catch (utils::ScException const & exception)
  {
    SCP_LOG_ERROR("Cannot unregister agent " << agentNode.Hash() << "\n" << exception.Description());
  }
}

}  // namespace scp
