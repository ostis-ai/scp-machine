/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpStringOperatorInterpreter.hpp"

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "scpStringOperatorInterpreter.hpp"
#include "string_operators/SCPOperatorStringSplit.hpp"
#include "string_operators/SCPOperatorStringReplace.hpp"
#include "string_operators/SCPOperatorStringSlice.hpp"
#include "string_operators/SCPOperatorContStringConcat.hpp"
#include "string_operators/SCPOperatorStringStartsWith.hpp"
#include "string_operators/SCPOperatorStringEndsWith.hpp"
#include "string_operators/SCPOperatorStringToLowerCase.hpp"
#include "string_operators/SCPOperatorStringToUpperCase.hpp"
#include "string_operators/SCPOperatorStringLen.hpp"
#include "string_operators/SCPOperatorStringSub.hpp"
#include "string_operators/SCPOperatorStringIfEq.hpp"
#include "string_operators/SCPOperatorStringIfGr.hpp"
#include "sc-memory/sc_memory.hpp"
#include <iostream>

namespace scp
{
ScAddr ASCPStringOperatorInterpreter::msAgentKeynode;

SC_AGENT_IMPLEMENTATION(ASCPStringOperatorInterpreter)
{
    if (!edgeAddr.IsValid())
            return SC_RESULT_ERROR;

        ScAddr scp_operator =m_memoryCtx.GetEdgeTarget(edgeAddr);

        ScAddr type;
        if (SC_TRUE != Utils::resolveOperatorType(m_memoryCtx, scp_operator, type))
            return SC_RESULT_ERROR_INVALID_TYPE;

        SCPOperator* oper = nullptr;
        if (type == Keynodes::op_stringSplit)
        {
            oper = new SCPOperatorStringSplit(m_memoryCtx, scp_operator);
        }
        if (type == Keynodes::op_stringSlice)
        {
            oper = new SCPOperatorStringSlice(m_memoryCtx, scp_operator);
        }
        if (type == Keynodes::op_stringReplace)
        {
            oper = new SCPOperatorStringReplace(m_memoryCtx, scp_operator);
        }
        if (type == Keynodes::op_contStringConcat)
        {
            oper = new SCPOperatorContStringConcat(m_memoryCtx, scp_operator);
        }
        if (type == Keynodes::op_stringIfEq)
        {
            oper = new SCPOperatorStringIfEq(m_memoryCtx, scp_operator);
        }
        if (type == Keynodes::op_stringIfGr)
        {
            oper = new SCPOperatorStringIfGr(m_memoryCtx, scp_operator);
        }
        if (type == Keynodes::op_stringLen)
        {
            oper = new SCPOperatorStringLen(m_memoryCtx, scp_operator);
        }
        if (type == Keynodes::op_stringSub)
        {
            oper = new SCPOperatorStringSub(m_memoryCtx, scp_operator);
        }
        if (type == Keynodes::op_stringStartsWith)
        {
            oper = new SCPOperatorStringStartsWith(m_memoryCtx, scp_operator);
        }
        if (type == Keynodes::op_stringEndsWith)
        {
            oper = new SCPOperatorStringEndsWith(m_memoryCtx, scp_operator);
        }
        if (type == Keynodes::op_stringToUpperCase)
        {
            oper = new SCPOperatorStringToUpperCase(m_memoryCtx, scp_operator);
        }
        if (type == Keynodes::op_stringToLowerCase)
        {
            oper = new SCPOperatorStringToLowerCase(m_memoryCtx, scp_operator);
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
