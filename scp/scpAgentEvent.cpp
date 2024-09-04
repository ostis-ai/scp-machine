/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "sc-memory/sc_addr.hpp"
#include "scp.hpp"
#include "scpKeynodes.hpp"
#include "scpAgentEvent.hpp"

extern "C"
{
#include "sc-core/sc-store/sc_event.h"
}

#include <iostream>

namespace scp {

concurrent_deque<SCPAgentEvent*> SCPAgentEvent::scp_agent_events;

void SCPAgentEvent::register_all_scp_agents(ScMemoryContext& ctx)
{
    ScIterator3Ptr iter_agent = ctx.Iterator3(Keynodes::active_sc_agent, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (iter_agent->Next())
    {
        ScAddr agent = iter_agent->Get(2);
        register_scp_agent(ctx, agent);
    }
}

void SCPAgentEvent::unregister_all_scp_agents(ScMemoryContext& ctx)
{
    while (!scp_agent_events.empty())
    {
        SCPAgentEvent* event = scp_agent_events.front();
        scp_agent_events.pop();
        delete event;
    }
}

void SCPAgentEvent::register_scp_agent(ScMemoryContext& ctx, ScAddr& agent_node)
{
    ScAddr abstract_agent, scp_agent;
    ScIterator3Ptr iter_impl = ctx.Iterator3(ScType::NodeConst, ScType::EdgeAccessConstPosPerm, agent_node);
    while (iter_impl->Next())
    {
        ScIterator5Ptr iter_agent = ctx.Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, iter_impl->Get(0), ScType::EdgeAccessConstPosPerm, Keynodes::nrel_inclusion);
        while (iter_agent->Next())
        {
            if (ctx.HelperCheckEdge(Keynodes::abstract_sc_agent, iter_agent->Get(0), ScType::EdgeAccessConstPosPerm))
            {
                abstract_agent = iter_agent->Get(0);
                scp_agent = iter_impl->Get(0);
                break;
            }
        }
    }
    if (!scp_agent.IsValid())
    {
      SCP_LOG_ERROR("Not found sc-agent for scp-agent \"" << ctx.HelperGetSystemIdtf(agent_node) << "\"");
      return;
    }
    if (!abstract_agent.IsValid())
    {
      SCP_LOG_ERROR("Not found abstract agent for scp-agent \"" << ctx.HelperGetSystemIdtf(agent_node) << "\"");
      return;
    }

    // Find program
    ScAddr agent_proc;
    ScIterator5Ptr iter_proc = ctx.Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, scp_agent, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_sc_agent_program);
    while (iter_proc->Next())
    {
        ScIterator3Ptr iter_proc2 = ctx.Iterator3(iter_proc->Get(0), ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
        while (iter_proc2->Next())
        {
            if (ctx.HelperCheckEdge(Keynodes::agent_scp_program, iter_proc2->Get(2), ScType::EdgeAccessConstPosPerm))
            {
                agent_proc = iter_proc2->Get(2);
                break;
            }
        }
    }
    if (!agent_proc.IsValid())
    {
      SCP_LOG_ERROR("Not found program for sc-agent \"" << ctx.HelperGetSystemIdtf(abstract_agent) << "\"");
      return;
    }

    // Old SCP program check
    iter_proc = ctx.Iterator5(agent_proc, ScType::EdgeAccessConstPosPerm, ScType::NodeVar, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_key_sc_element);
    if (!iter_proc->Next())
    {
      SCP_LOG_ERROR("Not found process variable in program for sc-agent \"" << ctx.HelperGetSystemIdtf(abstract_agent) << "\"");
      return;
    }

    // Find event
    ScAddr event_type_node, event_node;
    ScIterator5Ptr iter_event = ctx.Iterator5(abstract_agent, ScType::EdgeDCommonConst, ScType::Const, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_primary_initiation_condition);
    if (iter_event->Next())
    {
        event_type_node = ctx.GetEdgeSource(iter_event->Get(2));
        event_node = ctx.GetEdgeTarget(iter_event->Get(2));
    }
    else
    {
      SCP_LOG_ERROR("Not found primary initiation condition for sc-agent \"" << ctx.HelperGetSystemIdtf(abstract_agent) << "\"");
      return;
    }

    ScAddr action_addr;
    ScIterator5Ptr iter_action = ctx.Iterator5(abstract_agent, ScType::EdgeDCommonConst, ScType::Const, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_sc_agent_action_class);
    if (iter_action->Next())
      action_addr = iter_action->Get(2);
    else
      SCP_LOG_WARNING("Not found action class for sc-agent \"" << ctx.HelperGetSystemIdtf(abstract_agent) << "\"");

    scp_agent_events.push(new SCPAgentEvent(ctx, event_node, resolve_event_type(event_type_node), action_addr, agent_proc));

    SCP_LOG_INFO("Register scp-agent \"" << ctx.HelperGetSystemIdtf(abstract_agent) << "\"");
}

void SCPAgentEvent::unregister_scp_agent(ScMemoryContext& ctx, ScAddr& agent_node)
{
    ScAddr abstract_agent, scp_agent;
    ScIterator3Ptr iter_impl = ctx.Iterator3(ScType::NodeConst, ScType::EdgeAccessConstPosPerm, agent_node);
    while (iter_impl->Next())
    {
        ScIterator5Ptr iter_agent = ctx.Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, iter_impl->Get(0), ScType::EdgeAccessConstPosPerm, Keynodes::nrel_inclusion);
        while (iter_agent->Next())
        {
            if (ctx.HelperCheckEdge(Keynodes::abstract_sc_agent, iter_agent->Get(0), ScType::EdgeAccessConstPosPerm))
            {
                abstract_agent = iter_agent->Get(0);
                scp_agent = iter_impl->Get(0);
                break;
            }
        }
    }
    if (!scp_agent.IsValid())
    {
        SCP_LOG_ERROR("Not found sc-agent for scp-agent \"" << ctx.HelperGetSystemIdtf(agent_node) << "\"");
        return;
    }
    if (!abstract_agent.IsValid())
    {
        SCP_LOG_ERROR("Not found abstract agent for scp-agent \"" << ctx.HelperGetSystemIdtf(agent_node) << "\"");
        return;
    }

    // Find program
    ScAddr agent_proc;
    ScIterator5Ptr iter_proc = ctx.Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, scp_agent, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_sc_agent_program);
    while (iter_proc->Next())
    {
        ScIterator3Ptr iter_proc2 = ctx.Iterator3(iter_proc->Get(0), ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
        while (iter_proc2->Next())
        {
            if (ctx.HelperCheckEdge(Keynodes::agent_scp_program, iter_proc2->Get(2), ScType::EdgeAccessConstPosPerm))
            {
                agent_proc = iter_proc2->Get(2);
                break;
            }
        }
    }
    if (!agent_proc.IsValid())
    {
        SCP_LOG_ERROR("Not found program for sc-agent \"" << ctx.HelperGetSystemIdtf(abstract_agent) << "\"");
        return;
    }

    auto checker = [&agent_proc](SCPAgentEvent * event)
    {
        return event->GetProcAddr() == agent_proc;
    };

    SCPAgentEvent * event;
    if (scp_agent_events.extract(checker, event))
    {
        delete event;
        SCP_LOG_INFO("Unregister scp-agent \"" << ctx.HelperGetSystemIdtf(abstract_agent) << "\"");
    }
}

sc_event_type SCPAgentEvent::ConvertEventType(ScEvent::Type type)
{
    switch (type)
    {
        case ScEvent::Type::AddOutputEdge:
            return SC_EVENT_ADD_OUTPUT_ARC;

        case ScEvent::Type::AddInputEdge:
            return SC_EVENT_ADD_INPUT_ARC;

        case ScEvent::Type::RemoveOutputEdge:
            return SC_EVENT_REMOVE_OUTPUT_ARC;

        case ScEvent::Type::RemoveInputEdge:
            return SC_EVENT_REMOVE_INPUT_ARC;

        case ScEvent::Type::EraseElement:
            return SC_EVENT_REMOVE_ELEMENT;

        case ScEvent::Type::ContentChanged:
            return SC_EVENT_CONTENT_CHANGED;
    }

    SC_THROW_EXCEPTION(utils::ExceptionNotImplemented,
                       "Unsupported event type " + std::to_string(int(type)));
}

ScEvent::Type SCPAgentEvent::resolve_event_type(ScAddr const& event_type_node)
{
    if (event_type_node == Keynodes::sc_event_add_output_arc)
        return ScEvent::Type::AddOutputEdge;
    if (event_type_node == Keynodes::sc_event_add_input_arc)
        return ScEvent::Type::AddInputEdge;

    if (event_type_node == Keynodes::sc_event_remove_output_arc)
        return ScEvent::Type::RemoveOutputEdge;
    if (event_type_node == Keynodes::sc_event_remove_input_arc)
        return ScEvent::Type::RemoveInputEdge;

    if (event_type_node == Keynodes::sc_event_content_changed)
        return ScEvent::Type::ContentChanged;

    if (event_type_node == Keynodes::sc_event_remove_element)
        return ScEvent::Type::EraseElement;

    return ScEvent::Type::AddOutputEdge;
}

sc_result SCPAgentEvent::runSCPAgent(sc_event const* evt, sc_addr edge, sc_addr other_el)
{
    ScAddr quest(other_el);

    ScIterator5Ptr iter_quest = scpModule::s_default_ctx.Iterator5(quest, ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_authors);
    if (iter_quest->Next())
    {
        if (iter_quest->Get(2) == Keynodes::abstract_scp_machine)
            return SC_RESULT_OK;
    }

    auto * data = (ScAddr *)sc_event_get_data(evt);
    ScAddr action_addr = data[0];

    if (action_addr.IsValid() && !scpModule::s_default_ctx.HelperCheckEdge(action_addr, other_el, ScType::EdgeAccessConstPosPerm))
      return SC_RESULT_OK;

    ScAddr proc_addr = data[1];
    ScAddr inp_arc(edge);

    ScAddr scp_quest = scpModule::s_default_ctx.CreateNode(ScType::NodeConst);
    ScAddr arc1 = scpModule::s_default_ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, scp_quest, proc_addr);
    scpModule::s_default_ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::rrel_1, arc1);

    ScAddr scp_params = scpModule::s_default_ctx.CreateNode(ScType::NodeConst);
    arc1 = scpModule::s_default_ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, scp_params, proc_addr);
    scpModule::s_default_ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::rrel_1, arc1);

    arc1 = scpModule::s_default_ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, scp_params, inp_arc);
    scpModule::s_default_ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::rrel_2, arc1);

    arc1 = scpModule::s_default_ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, scp_quest, scp_params);
    scpModule::s_default_ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::rrel_2, arc1);

    arc1 = scpModule::s_default_ctx.CreateEdge(ScType::EdgeDCommonConst, scp_quest, Keynodes::abstract_scp_machine);
    scpModule::s_default_ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::nrel_authors, arc1);

    scpModule::s_default_ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::action_scp_interpretation_request, scp_quest);
    scpModule::s_default_ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::action_initiated, scp_quest);

    return SC_RESULT_OK;
}

SCPAgentEvent::SCPAgentEvent(ScMemoryContext& ctx, const ScAddr& addr, ScEvent::Type eventType, const ScAddr& actionClass, const ScAddr& procAddr)
{
    auto * data = new ScAddr[2];
    data[0] = actionClass;
    data[1] = procAddr;
    m_event = sc_event_new_ex(
        ctx.GetRealContext(),
        *addr,
        ConvertEventType(eventType),
        data,
        runSCPAgent,
        nullptr);
}

ScAddr SCPAgentEvent::GetProcAddr()
{
    auto * data = (ScAddr *)sc_event_get_data(m_event);
    return data[1];
}

SCPAgentEvent::~SCPAgentEvent()
{
    if (m_event)
    {
        delete[] (ScAddr *)sc_event_get_data(m_event);
        sc_event_destroy(m_event);
    }
}

}

