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

SC_AGENT_IMPLEMENTATION(ASCPProcessCreator)
{
    if (!edgeAddr.IsValid())
        return SC_RESULT_ERROR;

    ScAddr action =m_memoryCtx.GetEdgeTarget(edgeAddr);

    if (!m_memoryCtx.HelperCheckEdge(Keynodes::question_scp_interpretation_request, action, ScType::EdgeAccessConstPosPerm))
        return SC_RESULT_ERROR_INVALID_PARAMS;

    ScAddr program, params;
    ScIterator5Ptr iter_param =m_memoryCtx.Iterator5(action, ScType::EdgeAccessConstPosPerm, ScType::Node, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_1);
    if (iter_param->Next())
        program = iter_param->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    iter_param =m_memoryCtx.Iterator5(action, ScType::EdgeAccessConstPosPerm, ScType::Node, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_2);
    if (iter_param->Next())
        params = iter_param->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    ScTemplate program_templ;
   m_memoryCtx.HelperBuildTemplate(program_templ, program);
    ScTemplateParams gen_params;

    ScAddr process_node;
    ScIterator5Ptr iter_temp =m_memoryCtx.Iterator5(program, ScType::EdgeAccessConstPosPerm, ScType::NodeVar, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_key_sc_element);
    if (iter_temp->IsValid() && iter_temp->Next())
        process_node = iter_temp->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    iter_temp =m_memoryCtx.Iterator5(process_node, ScType::EdgeAccessVarPosPerm, ScType::NodeVar, ScType::EdgeAccessConstPosPerm, program);
    if (!iter_temp->IsValid())
        return SC_RESULT_ERROR_INVALID_PARAMS;

    //!TODO Make all sc-links constant to allow using constant sc-links within scp-program code
    while (iter_temp->Next())
    {
        ScAddr order;
        if (Utils::resolveOrderRoleRelation(m_memoryCtx, iter_temp->Get(1), order))
        {
            iter_param =m_memoryCtx.Iterator5(params, ScType::EdgeAccessConstPosPerm, ScType(0), ScType::EdgeAccessConstPosPerm, order);
            if (!iter_param->Next())
            {
#ifdef SCP_DEBUG
                Utils::logSCPError(m_memoryCtx, "Missed scp-program parameter", order);
#endif
                continue;
            }
            gen_params.Add(iter_temp->Get(2), iter_param->Get(2));
        }
    }

    ScTemplateGenResult result;
   m_memoryCtx.HelperGenTemplate(program_templ, result, gen_params);

    ScAddr const_process_node = result[process_node];
    iter_temp =m_memoryCtx.Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, const_process_node, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_decomposition_of_action);
    if (iter_temp->Next())
    {
        ScIterator5Ptr oper_iter =m_memoryCtx.Iterator5(iter_temp->Get(0), ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_1);
        if (oper_iter->Next())
        {
            ScAddr arc =m_memoryCtx.CreateEdge(ScType::EdgeDCommonConst, action, const_process_node);
           m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::nrel_result, arc);
           m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::active_action, oper_iter->Get(2));
        }
        else
        {
#ifdef SCP_DEBUG
            Utils::logSCPError(m_memoryCtx, "Missed initial scp-operator", program);
#endif
           m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::question_finished, const_process_node);
        }
    }
    else
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(m_memoryCtx, "Missed scp-process decomposition", program);
#endif
       m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::question_finished, const_process_node);
    }

    return SC_RESULT_OK;
}

}
