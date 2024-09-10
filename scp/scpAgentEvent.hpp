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
  void static register_all_scp_agents(ScAgentContext & ctx);
  void static unregister_all_scp_agents(ScAgentContext & ctx);
  void static handle_all_active_agents(
      ScAgentContext & ctx,
      std::function<void(ScAgentContext &, ScAddr const &)> const & handler);
  void static register_scp_agent(ScAgentContext & ctx, ScAddr const & agentNode);
  void static unregister_scp_agent(ScAgentContext & ctx, ScAddr const & agentNode);

  static void handle_active_agent(
      ScAgentContext & ctx,
      std::function<void(ScAgentContext &, ScAddr const &)> const & handler,
      ScAddr const & agent);
};

}  // namespace scp
