/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "scpProcessControlOperatorInterpreter.hpp"
#include "process_control_operators/SCPOperatorReturn.hpp"
#include "process_control_operators/SCPOperatorSysWait.hpp"
#include "process_control_operators/SCPOperatorCall.hpp"
#include "process_control_operators/SCPOperatorWaitReturn.hpp"
#include "sc-memory/sc-memory/sc_memory.hpp"
#include <iostream>

namespace scp {
ScAddr ASCPProcessControlOperatorInterpreter::msAgentKeynode;

SC_AGENT_IMPLEMENTATION(ASCPProcessControlOperatorInterpreter)
{
    if (!edgeAddr.IsValid())
        return SC_RESULT_ERROR;

    ScAddr scp_operator = ms_context->GetEdgeTarget(edgeAddr);

    ScAddr type;
    if (SC_TRUE != Utils::resolveOperatorType(ms_context, scp_operator, type))
        return SC_RESULT_ERROR_INVALID_TYPE;

    SCPOperator* oper;
    if (type == Keynodes::op_return)
    {
        oper = new SCPOperatorReturn(ms_context, scp_operator);
    }
    else if (type == Keynodes::op_sys_wait)
    {
        oper = new SCPOperatorSysWait(ms_context, scp_operator);
    }
    else if (type == Keynodes::op_call)
    {
        oper = new SCPOperatorCall(ms_context, scp_operator);
    }
    else if (type == Keynodes::op_waitReturn)
    {
        oper = new SCPOperatorWaitReturn(ms_context, scp_operator);
    }
    else
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
