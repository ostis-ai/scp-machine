/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "scpGenOperatorInterpreter.hpp"
#include "gen_operators/SCPOperatorGenEl.hpp"
#include "gen_operators/SCPOperatorGenElStr3.hpp"
#include "gen_operators/SCPOperatorGenElStr5.hpp"
#include "sc-memory/sc-memory/sc_memory.hpp"
#include <iostream>

namespace scp {
ScAddr ASCPGenOperatorInterpreter::msAgentKeynode;

SC_AGENT_IMPLEMENTATION(ASCPGenOperatorInterpreter)
{
    if (!edgeAddr.IsValid())
        return SC_RESULT_ERROR;

    ScAddr scp_operator = ms_context->GetEdgeTarget(edgeAddr);

    ScAddr type;
    if (SC_TRUE != Utils::resolveOperatorType(ms_context, scp_operator, type))
        return SC_RESULT_ERROR_INVALID_TYPE;

    SCPOperator* oper = nullptr;
    if (type == Keynodes::op_genEl)
    {
        oper = new SCPOperatorGenEl(ms_context, scp_operator);
    }
    if (type == Keynodes::op_genElStr3)
    {
        oper = new SCPOperatorGenElStr3(ms_context, scp_operator);
    }
    if (type == Keynodes::op_genElStr5)
    {
        oper = new SCPOperatorGenElStr5(ms_context, scp_operator);
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
