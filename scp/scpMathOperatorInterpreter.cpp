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

namespace scp
{
ScAddrToValueUnorderedMap<std::function<SCPOperator *(ScMemoryContext &, ScAddr)>>
    ASCPMathOperatorInterpreter::supportedOperators = {
        {Keynodes::op_contSin,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorSIn(ctx, addr);
         }},
        {Keynodes::op_contCos,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorCos(ctx, addr);
         }},
        {Keynodes::op_contTg,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorTg(ctx, addr);
         }},
        {Keynodes::op_contASin,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorASin(ctx, addr);
         }},
        {Keynodes::op_contACos,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorACos(ctx, addr);
         }},
        {Keynodes::op_contATg,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorATg(ctx, addr);
         }},
        {Keynodes::op_contDivInt,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorDivInt(ctx, addr);
         }},
        {Keynodes::op_contDivRem,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorDivRem(ctx, addr);
         }},
        {Keynodes::op_ifEq,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorIfEq(ctx, addr);
         }},
        {Keynodes::op_contLn,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorContLn(ctx, addr);
         }},
        {Keynodes::op_ifGr,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorIfGr(ctx, addr);
         }},
        {Keynodes::op_contAdd,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorContAdd(ctx, addr);
         }},
        {Keynodes::op_contSub,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorContSub(ctx, addr);
         }},
        {Keynodes::op_contMult,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorContMult(ctx, addr);
         }},
        {Keynodes::op_contDiv,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorContDiv(ctx, addr);
         }},
        {Keynodes::op_contPow,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorContPow(ctx, addr);
         }},
};

ScResult ASCPMathOperatorInterpreter::DoProgram(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event,
    ScAction & action)
{
  if (!event.GetArc().IsValid())
    return action.FinishUnsuccessfully();

  ScAddr scp_operator = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scp_operator, type))
    return action.FinishUnsuccessfully();

  SCPOperator * oper = nullptr;

  if (supportedOperators.count(type))
    oper = supportedOperators.at(type)(m_context, scp_operator);

  if (oper == nullptr)
  {
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
  // todo(codegen-removal): replace action with your action class
  return ScKeynodes::action;
}

ScAddr ASCPMathOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}

bool ASCPMathOperatorInterpreter::CheckInitiationCondition(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event)
{
  ScAddr scp_operator = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scp_operator, type))
    return false;
  return supportedOperators.count(type);
}

}  // namespace scp
