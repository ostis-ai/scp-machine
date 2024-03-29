/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "scpSearchOperatorInterpreter.hpp"
#include "search_operators/SCPOperatorSearchElStr3.hpp"
#include "search_operators/SCPOperatorSearchElStr5.hpp"
#include "search_operators/SCPOperatorSearchSetStr3.hpp"
#include "search_operators/SCPOperatorSearchSetStr5.hpp"
#include "search_operators/SCPOperatorSysSearch.hpp"
#include "sc-memory/sc_memory.hpp"
#include <iostream>

namespace scp {
ScAddr ASCPSearchOperatorInterpreter::msAgentKeynode;

SC_AGENT_IMPLEMENTATION(ASCPSearchOperatorInterpreter)
{
    if (!edgeAddr.IsValid())
        return SC_RESULT_ERROR;

    ScAddr scp_operator =m_memoryCtx.GetEdgeTarget(edgeAddr);

    ScAddr type;
    if (SC_TRUE != Utils::resolveOperatorType(m_memoryCtx, scp_operator, type))
        return SC_RESULT_ERROR_INVALID_TYPE;

    SCPOperator* oper = nullptr;
    if (type == Keynodes::op_searchElStr3)
    {
        oper = new SCPOperatorSearchElStr3(m_memoryCtx, scp_operator);
    }
    if (type == Keynodes::op_searchElStr5)
    {
        oper = new SCPOperatorSearchElStr5(m_memoryCtx, scp_operator);
    }
    if (type == Keynodes::op_searchSetStr3)
    {
        oper = new SCPOperatorSearchSetStr3(m_memoryCtx, scp_operator);
    }
    if (type == Keynodes::op_searchSetStr5)
    {
        oper = new SCPOperatorSearchSetStr5(m_memoryCtx, scp_operator);
    }
    if (type == Keynodes::op_sys_search)
    {
        oper = new SCPOperatorSysSearch(m_memoryCtx, scp_operator);
    }
    if (oper == nullptr)
        return SC_RESULT_ERROR_INVALID_PARAMS;

#ifdef SCP_DEBUG
    std::cout << oper->GetTypeName() << std::endl;
#endif
    sc_result parse_result = oper->Parse();
    if (parse_result != SC_RESULT_OK)
    {
        delete oper;
        return parse_result;
    }
    else
    {
        sc_result execute_result;
        execute_result = oper->Execute();
        delete oper;
        return execute_result;
    }
}

}
