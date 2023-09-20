/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "SCPOperatorReturn.hpp"
#include "sc-memory/sc-memory/sc_memory.hpp"
#include <iostream>

namespace scp {

//return
SCPOperatorReturn::SCPOperatorReturn(const std::unique_ptr<ScMemoryContext> &ctx, ScAddr addr): SCPOperator(ctx, addr)
{
}

std::string SCPOperatorReturn::GetTypeName()
{
    return "return";
}

sc_result SCPOperatorReturn::Parse()
{
    return SC_RESULT_OK;
}

sc_result SCPOperatorReturn::Execute()
{
    sc_bool flag = SC_FALSE;
    ScAddr process_node;
    ScIterator3Ptr iter1 = ms_context->Iterator3(ScType::NodeConst, ScType::EdgeAccessConstPosPerm, this->addr);
    while (iter1->Next())
    {
        ScIterator5Ptr iter2 = ms_context->Iterator5(iter1->Get(0), ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_decomposition_of_action);
        if (iter2->Next())
        {
            process_node = iter2->Get(2);
            flag = SC_TRUE;
            break;
        }
    }
    if (SC_FALSE == flag)
    {
        FinishExecutionWithError();
    }
    else
    {
        ScIterator5Ptr iter3 = ms_context->Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, process_node, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_result);
        if (iter3->Next())
        {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::question_finished, iter3->Get(0));
        }
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::question_finished, process_node);
    }
    return SC_RESULT_OK;
}
}
