/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent_context.hpp>

namespace scp
{

class SCPAgentEvent
{
public:
  void static SubscribeAllSCPAgents(ScAgentContext & ctx);
  void static UnsubscribeAllSCPAgents(ScAgentContext & ctx);
  void static HandleAllActiveAgents(
      ScAgentContext & ctx,
      std::function<void(ScAgentContext &, ScAddr const &)> const & handler);
  void static RegisterSCPAgent(ScAgentContext & ctx, ScAddr const & agentNode);
  void static UnregisterSCPAgent(ScAgentContext & ctx, ScAddr const & agentNode);

  static void HandleActiveAgent(
      ScAgentContext & ctx,
      std::function<void(ScAgentContext &, ScAddr const &)> const & handler,
      ScAddr const & agent);
};

}  // namespace scp
