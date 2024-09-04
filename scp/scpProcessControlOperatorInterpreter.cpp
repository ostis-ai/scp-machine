/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpProcessControlOperatorInterpreter.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "process_control_operators/SCPOperatorReturn.hpp"
#include "process_control_operators/SCPOperatorSysWait.hpp"
#include "process_control_operators/SCPOperatorCall.hpp"
#include "process_control_operators/SCPOperatorWaitReturn.hpp"

#include <iostream>

namespace scp {
ScResult ASCPProcessControlOperatorInterpreter::DoProgram(ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event, ScAction & action)
{
    if (!event.GetArc().IsValid())
        return action.FinishUnsuccessfully();

    ScAddr scp_operator = event.GetOtherElement();

    ScAddr type;
    if (SC_TRUE != Utils::resolveOperatorType(m_context, scp_operator, type))
        return action.FinishUnsuccessfully();

    SCPOperator* oper;
    if (type == Keynodes::op_return)
    {
        oper = new SCPOperatorReturn(m_context, scp_operator);
    }
    else if (type == Keynodes::op_sys_wait)
    {
        oper = new SCPOperatorSysWait(m_context, scp_operator);
    }
    else if (type == Keynodes::op_call)
    {
        oper = new SCPOperatorCall(m_context, scp_operator);
    }
    else if (type == Keynodes::op_waitReturn)
    {
        oper = new SCPOperatorWaitReturn(m_context, scp_operator);
    }
    else
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

ScAddr ASCPProcessControlOperatorInterpreter::GetActionClass() const
{
//todo(codegen-removal): replace action with your action class
  return ScKeynodes::action;
}

ScAddr ASCPProcessControlOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}

}
