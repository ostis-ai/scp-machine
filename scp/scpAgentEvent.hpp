/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/sc-memory/sc_addr.hpp"
#include "sc-memory/sc-memory/sc_event.hpp"
#include "concurrent_queue.hpp"

namespace scp {

class SCPAgentEvent
{
public:
    static concurrent_deque<SCPAgentEvent*> scp_agent_events;
    void static register_all_scp_agents(std::unique_ptr<ScMemoryContext>& ctx);
    void static unregister_all_scp_agents(std::unique_ptr<ScMemoryContext>& ctx);
    void static register_scp_agent(std::unique_ptr<ScMemoryContext>& ctx, ScAddr& agent_node);
    void static unregister_scp_agent(std::unique_ptr<ScMemoryContext>& ctx, ScAddr& agent_node);
    ScAddr GetProcAddr();
    ~SCPAgentEvent();

private:
    sc_event_type ConvertEventType(ScEvent::Type type);
    ScEvent::Type static resolve_event_type(ScAddr const& event_type_node);
    sc_result static runSCPAgent(sc_event const* evt, sc_addr edge, sc_addr other_el);
    SCPAgentEvent(const std::unique_ptr<ScMemoryContext>& ctx, const ScAddr& addr, ScEvent::Type eventType, const ScAddr& actionClass, const ScAddr& procAddr);
    sc_event* m_event;
};

}

