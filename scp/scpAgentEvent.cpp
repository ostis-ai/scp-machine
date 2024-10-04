/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpAgentEvent.hpp"

#include "scp.hpp"
#include "scpHandlingEventThatInitiatesAgentSCPProgram.hpp"
#include "scpKeynodes.hpp"

namespace scp
{
void SCPAgentEvent::SubscribeAllSCPAgents(ScAgentContext & ctx)
{
  HandleAllActiveAgents(ctx, RegisterSCPAgent);
}

void SCPAgentEvent::UnsubscribeAllSCPAgents(ScAgentContext & ctx)
{
  HandleAllActiveAgents(ctx, UnregisterSCPAgent);
}

void SCPAgentEvent::HandleAllActiveAgents(
    ScAgentContext & ctx,
    std::function<void(ScAgentContext &, ScAddr const &)> const & handler)
{
  auto const & activeAgentsIterator =
      ctx.CreateIterator3(Keynodes::active_sc_agent, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
  while (activeAgentsIterator->Next())
  {
    ScAddr const & agent = activeAgentsIterator->Get(2);
    HandleActiveAgent(ctx, handler, agent);
  }
}

void SCPAgentEvent::HandleActiveAgent(
    ScAgentContext & ctx,
    std::function<void(ScAgentContext &, ScAddr const &)> const & handler,
    ScAddr const & agent)
{
  auto const & agentImplementationsIterator =
      ctx.CreateIterator3(ScType::NodeConst, ScType::EdgeAccessConstPosPerm, agent);
  while (agentImplementationsIterator->Next())
  {
    ScAddr const & agentImplementation = agentImplementationsIterator->Get(0);
    if (agentImplementation == Keynodes::active_sc_agent)
      continue;
    if (ctx.CheckConnector(
            Keynodes::platform_independent_abstract_sc_agent, agentImplementation, ScType::EdgeAccessConstPosPerm))
      handler(ctx, agentImplementation);
  }
}

void SCPAgentEvent::RegisterSCPAgent(ScAgentContext & ctx, ScAddr const & agentNode)
{
  try
  {
    ctx.SubscribeSpecifiedAgent<ASCPHandlingEventThatInitiatesAgentSCPProgram>(agentNode);
  }
  catch (utils::ScException const & exception)
  {
    SCP_LOG_ERROR("Cannot register agent " << agentNode.Hash() << "\n" << exception.Description());
  }
}

void SCPAgentEvent::UnregisterSCPAgent(ScAgentContext & ctx, ScAddr const & agentNode)
{
  try
  {
    ctx.UnsubscribeSpecifiedAgent<ASCPHandlingEventThatInitiatesAgentSCPProgram>(agentNode);
  }
  catch (utils::ScException const & exception)
  {
    SCP_LOG_ERROR("Cannot unregister agent " << agentNode.Hash() << "\n" << exception.Description());
  }
}

}  // namespace scp
