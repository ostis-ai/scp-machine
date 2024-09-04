#include "scpMathOperatorInterpreter.hpp"

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "math_operators/SCPOperatorSin.hpp"
#include "math_operators/SCPOperatorCos.hpp"
#include "math_operators/SCPOperatorTg.hpp"
#include "math_operators/SCPOperatorASin.hpp"
#include "math_operators/SCPOperatorACos.hpp"
#include "math_operators/SCPOperatorATg.hpp"
#include "math_operators/SCPOperatorDivInt.hpp"
#include "math_operators/SCPOperatorDivRem.hpp"
#include "math_operators/SCPOperatorIfEq.hpp"
#include "math_operators/SCPOperatorIfGr.hpp"
#include "math_operators/SCPOperatorContLn.hpp"
#include "math_operators/SCPOperatorContAdd.hpp"
#include "math_operators/SCPOperatorContSub.hpp"
#include "math_operators/SCPOperatorContMult.hpp"
#include "math_operators/SCPOperatorContPow.hpp"
#include "math_operators/SCPOperatorContDiv.hpp"

#include <iostream>
#include <string>

namespace scp {
ScResult ASCPMathOperatorInterpreter::DoProgram(ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event, ScAction & action){
    if (!event.GetArc().IsValid())
        return action.FinishUnsuccessfully();

    ScAddr scp_operator = event.GetOtherElement();

    ScAddr type;
    if (SC_TRUE != Utils::resolveOperatorType(m_context, scp_operator, type))
        return action.FinishUnsuccessfully();

    SCPOperator* oper = nullptr;
    if (type == Keynodes::op_contSin)
    {
        oper = new SCPOperatorSIn(m_context, scp_operator);
    }
    if (type == Keynodes::op_contCos)
    {
        oper = new SCPOperatorCos(m_context, scp_operator);
    }
    if (type == Keynodes::op_contTg)
    {
        oper = new SCPOperatorTg(m_context, scp_operator);
    }
    if (type == Keynodes::op_contASin)
    {
        oper = new SCPOperatorASin(m_context, scp_operator);
    }
    if (type == Keynodes::op_contACos)
    {
        oper = new SCPOperatorACos(m_context, scp_operator);
    }
    if (type == Keynodes::op_contATg)
    {
        oper = new SCPOperatorATg(m_context, scp_operator);
    }
    if (type == Keynodes::op_contDivInt)
    {
        oper = new SCPOperatorDivInt(m_context, scp_operator);
    }
    if (type == Keynodes::op_contDivRem)
    {
        oper = new SCPOperatorDivRem(m_context, scp_operator);
    }
    if (type == Keynodes::op_ifEq)
    {
        oper = new SCPOperatorIfEq(m_context, scp_operator);
    }
    if (type == Keynodes::op_contLn)
    {
        oper = new SCPOperatorContLn(m_context, scp_operator);
    }
    if (type == Keynodes::op_ifGr)
    {
        oper = new SCPOperatorIfGr(m_context, scp_operator);
    }
    if (type == Keynodes::op_contAdd)
    {
        oper = new SCPOperatorContAdd(m_context, scp_operator);
    }
    if (type == Keynodes::op_contSub)
    {
        oper = new SCPOperatorContSub(m_context, scp_operator);
    }
    if (type == Keynodes::op_contMult)
    {
        oper = new SCPOperatorContMult(m_context, scp_operator);
    }
    if (type == Keynodes::op_contDiv)
    {
        oper = new SCPOperatorContDiv(m_context, scp_operator);
    }
    if (type == Keynodes::op_contPow)
    {
        oper = new SCPOperatorContPow(m_context, scp_operator);
    }

    if (oper == nullptr){
        return action.FinishUnsuccessfully();
    }


    std::cout << oper->GetTypeName() << std::endl;

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

ScAddr ASCPMathOperatorInterpreter::GetActionClass() const
{
//todo(codegen-removal): replace action with your action class
  return ScKeynodes::action;
}

ScAddr ASCPMathOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}

}
