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

#include <iostream>

namespace scp
{
ScResult ASCPStringOperatorInterpreter::DoProgram(ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event, ScAction & action)
{
    if (!event.GetArc().IsValid())
            return action.FinishUnsuccessfully();

        ScAddr scp_operator = event.GetOtherElement();

        ScAddr type;
        if (SC_TRUE != Utils::resolveOperatorType(m_context, scp_operator, type))
            return action.FinishUnsuccessfully();

        SCPOperator* oper = nullptr;
        if (type == Keynodes::op_stringSplit)
        {
            oper = new SCPOperatorStringSplit(m_context, scp_operator);
        }
        if (type == Keynodes::op_stringSlice)
        {
            oper = new SCPOperatorStringSlice(m_context, scp_operator);
        }
        if (type == Keynodes::op_stringReplace)
        {
            oper = new SCPOperatorStringReplace(m_context, scp_operator);
        }
        if (type == Keynodes::op_contStringConcat)
        {
            oper = new SCPOperatorContStringConcat(m_context, scp_operator);
        }
        if (type == Keynodes::op_stringIfEq)
        {
            oper = new SCPOperatorStringIfEq(m_context, scp_operator);
        }
        if (type == Keynodes::op_stringIfGr)
        {
            oper = new SCPOperatorStringIfGr(m_context, scp_operator);
        }
        if (type == Keynodes::op_stringLen)
        {
            oper = new SCPOperatorStringLen(m_context, scp_operator);
        }
        if (type == Keynodes::op_stringSub)
        {
            oper = new SCPOperatorStringSub(m_context, scp_operator);
        }
        if (type == Keynodes::op_stringStartsWith)
        {
            oper = new SCPOperatorStringStartsWith(m_context, scp_operator);
        }
        if (type == Keynodes::op_stringEndsWith)
        {
            oper = new SCPOperatorStringEndsWith(m_context, scp_operator);
        }
        if (type == Keynodes::op_stringToUpperCase)
        {
            oper = new SCPOperatorStringToUpperCase(m_context, scp_operator);
        }
        if (type == Keynodes::op_stringToLowerCase)
        {
            oper = new SCPOperatorStringToLowerCase(m_context, scp_operator);
        }
        if (oper == nullptr)
            return action.FinishUnsuccessfully();

#ifdef SCP_DEBUG
    std::cout << oper->GetTypeName() << std::endl;
#endif

    sc_result parse_result = oper->Parse();
    if (parse_result != SC_RESULT_OK)
    {
        delete oper;
        return action.FinishUnsuccessfully();
    }
    else
    {
        sc_result execute_result;
        execute_result = oper->Execute();
        delete oper;
        return (execute_result == SC_RESULT_OK) ? action.FinishSuccessfully() : action.FinishUnsuccessfully();
    }
}

ScAddr ASCPStringOperatorInterpreter::GetActionClass() const
{
//todo(codegen-removal): replace action with your action class
  return ScKeynodes::action;
}

ScAddr ASCPStringOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}

}
