/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "scpOperator.hpp"
#include "scpFinishedInterpretationActionProcessor.hpp"
#include "sc-memory/sc-memory/sc_memory.hpp"
#include <iostream>

namespace scp {
ScAddr ASCPFinishedInterpretationActionProcessor::msAgentKeynode;

SC_AGENT_IMPLEMENTATION(ASCPFinishedInterpretationActionProcessor)
{
    if (!edgeAddr.IsValid())
        return SC_RESULT_ERROR;

    ScAddr scp_action = ms_context->GetEdgeTarget(edgeAddr);
    if (!ms_context->HelperCheckEdge(Keynodes::question_scp_interpretation_request, scp_action, ScType::EdgeAccessConstPosPerm))
        return SC_RESULT_ERROR_INVALID_PARAMS;

    ScAddr wait_operator;

    //SCP_CONST case
    ScIterator5Ptr iter = ms_context->Iterator5(ScType::NodeConst, ScType::EdgeAccessConstPosPerm, scp_action, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_scp_const);
    while (iter->Next())
    {
        wait_operator = iter->Get(0);
        if (!ms_context->HelperCheckEdge(Keynodes::op_waitReturn, wait_operator, ScType::EdgeAccessConstPosPerm) || !ms_context->HelperCheckEdge(Keynodes::active_action, wait_operator, ScType::EdgeAccessConstPosPerm))
            continue;
        SCPOperator::FinishExecutionSuccessfully(ms_context, wait_operator);
    }

    //SCP_VAR case
    ScIterator3Ptr iter3 = ms_context->Iterator3(ScType::NodeConst, ScType::EdgeAccessConstPosTemp, scp_action);
    while (iter3->Next())
    {
        ScIterator5Ptr iter5 = ms_context->Iterator5(ScType::NodeConst, ScType::EdgeAccessConstPosPerm, iter3->Get(0), ScType::EdgeAccessConstPosPerm, Keynodes::rrel_scp_var);
        while (iter5->Next())
        {
            wait_operator = iter5->Get(0);
            if (!ms_context->HelperCheckEdge(Keynodes::op_waitReturn, wait_operator, ScType::EdgeAccessConstPosPerm) || !ms_context->HelperCheckEdge(Keynodes::active_action, wait_operator, ScType::EdgeAccessConstPosPerm))
                continue;
            SCPOperator::FinishExecutionSuccessfully(ms_context, wait_operator);
        }
    }

    return SC_RESULT_OK;
}

}
