/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "scpProcessCreator.hpp"
#include "sc-memory/sc_memory.hpp"
#include <iostream>

namespace scp {
ScAddr ASCPProcessCreator::msAgentKeynode;

ScResult ASCPProcessCreator::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
    if (!event.GetArc().IsValid())
        return action.FinishUnsuccessfully();

    ScAddr action =m_context.GetEdgeTarget(event.GetArc());

    if (!m_context.HelperCheckEdge(Keynodes::action_scp_interpretation_request, action, ScType::EdgeAccessConstPosPerm))
        return action.FinishUnsuccessfully();

    ScAddr program, params;
    ScIterator5Ptr iter_param =m_context.Iterator5(action, ScType::EdgeAccessConstPosPerm, ScType::Node, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_1);
    if (iter_param->Next())
        program = iter_param->Get(2);
    else
        return action.FinishUnsuccessfully();

    iter_param =m_context.Iterator5(action, ScType::EdgeAccessConstPosPerm, ScType::Node, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_2);
    if (iter_param->Next())
        params = iter_param->Get(2);
    else
        return action.FinishUnsuccessfully();

    ScTemplate program_templ;
   m_context.HelperBuildTemplate(program_templ, program);
    ScTemplateParams gen_params;

    ScAddr process_node;
    ScIterator5Ptr iter_temp =m_context.Iterator5(program, ScType::EdgeAccessConstPosPerm, ScType::NodeVar, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_key_sc_element);
    if (iter_temp->IsValid() && iter_temp->Next())
        process_node = iter_temp->Get(2);
    else
        return action.FinishUnsuccessfully();

    iter_temp =m_context.Iterator5(process_node, ScType::EdgeAccessVarPosPerm, ScType::NodeVar, ScType::EdgeAccessConstPosPerm, program);
    if (!iter_temp->IsValid())
        return action.FinishUnsuccessfully();

    //!TODO Make all sc-links constant to allow using constant sc-links within scp-program code
    while (iter_temp->Next())
    {
        ScAddr order;
        if (Utils::resolveOrderRoleRelation(m_context, iter_temp->Get(1), order))
        {
            iter_param =m_context.Iterator5(params, ScType::EdgeAccessConstPosPerm, ScType(0), ScType::EdgeAccessConstPosPerm, order);
            if (!iter_param->Next())
            {
#ifdef SCP_DEBUG
                Utils::logSCPError(m_context, "Missed scp-program parameter", order);
#endif
                continue;
            }
            gen_params.Add(iter_temp->Get(2), iter_param->Get(2));
        }
    }

    ScTemplateGenResult result;
   m_context.HelperGenTemplate(program_templ, result, gen_params);

    ScAddr const_process_node = result[process_node];
    iter_temp =m_context.Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, const_process_node, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_decomposition_of_action);
    if (iter_temp->Next())
    {
        ScIterator5Ptr oper_iter =m_context.Iterator5(iter_temp->Get(0), ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_1);
        if (oper_iter->Next())
        {
            ScAddr arc =m_context.CreateEdge(ScType::EdgeDCommonConst, action, const_process_node);
           m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::nrel_result, arc);
           m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::active_action, oper_iter->Get(2));
        }
        else
        {
#ifdef SCP_DEBUG
            Utils::logSCPError(m_context, "Missed initial scp-operator", program);
#endif
           m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::action_finished, const_process_node);
        }
    }
    else
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(m_context, "Missed scp-process decomposition", program);
#endif
       m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::action_finished, const_process_node);
    }

    return action.FinishSuccessfully();
}

ScAddr ASCPProcessCreator::GetActionClass() const
{
//todo(codegen-removal): replace action with your action class
  return ScKeynodes::action;
}


}
