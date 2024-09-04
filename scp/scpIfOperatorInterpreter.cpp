/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "scpIfOperatorInterpreter.hpp"
#include "if_operators/SCPOperatorIfCoin.hpp"
#include "if_operators/SCPOperatorIfType.hpp"
#include "if_operators/SCPOperatorIfVarAssign.hpp"
#include "sc-memory/sc_memory.hpp"
#include <iostream>

namespace scp
{
ScAddr ASCPIfOperatorInterpreter::msAgentKeynode;

ScResult ASCPIfOperatorInterpreter::DoProgram(ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event, ScAction & action)
{
    if (!event.GetArc().IsValid())
        return action.FinishUnsuccessfully();

    ScAddr scp_operator =m_context.GetEdgeTarget(event.GetArc());

    ScAddr type;
    if (SC_TRUE != Utils::resolveOperatorType(m_context, scp_operator, type))
        return action.FinishUnsuccessfully();

    SCPOperator* oper = nullptr;
    if (type == Keynodes::op_ifCoin)
    {
        oper = new SCPOperatorIfCoin(m_context, scp_operator);
    }
    if (type == Keynodes::op_ifType)
    {
        oper = new SCPOperatorIfType(m_context, scp_operator);
    }
    if (type == Keynodes::op_ifVarAssign)
    {
        oper = new SCPOperatorIfVarAssign(m_context, scp_operator);
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
        return (parse_result == SC_RESULT_OK) ? action.FinishSuccessfully() : action.FinishUnsuccessfully();
    }
    else
    {
        sc_result execute_result;
        execute_result = oper->Execute();
        delete oper;
        return (execute_result == SC_RESULT_OK) ? action.FinishSuccessfully() : action.FinishUnsuccessfully();
    }
}

ScAddr ASCPIfOperatorInterpreter::GetActionClass() const
{
//todo(codegen-removal): replace action with your action class
  return ScKeynodes::action;
}

ScAddr ASCPIfOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}

}
