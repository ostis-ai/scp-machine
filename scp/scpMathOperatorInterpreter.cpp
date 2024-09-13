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
ScAddrToValueUnorderedMap<std::function<SCPOperator *(ScAgentContext &, ScAddr)>>
    ASCPMathOperatorInterpreter::supportedOperators = {};

ScResult ASCPMathOperatorInterpreter::DoProgram(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event,
    ScAction & action)
{
  ScAddr const & scpOperatorAddr = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scpOperatorAddr, type))
  {
    SC_AGENT_LOG_ERROR("Cannot resolve operator type for " << scpOperatorAddr.Hash());
    return action.FinishUnsuccessfully();
  }

  SCPOperator * scpOperator = nullptr;

  auto const & pair = supportedOperators.find(type);
  if (pair != supportedOperators.cend())
    scpOperator = pair->second(m_context, scpOperatorAddr);

  if (scpOperator == nullptr)
  {
    SC_AGENT_LOG_ERROR("Cannot create operator from " << scpOperatorAddr.Hash());
    return action.FinishUnsuccessfully();
  }

  std::cout << scpOperator->GetTypeName() << std::endl;

  sc_result parseResult = scpOperator->Parse();
  if (parseResult != SC_RESULT_OK)
  {
    SC_AGENT_LOG_ERROR("Cannot parse operator " << scpOperatorAddr.Hash());
    delete scpOperator;
    return action.FinishUnsuccessfully();
  }
  else
  {
    sc_result executeResult;
    executeResult = scpOperator->Execute();
    delete scpOperator;
    return (executeResult == SC_RESULT_OK) ? action.FinishSuccessfully() : action.FinishUnsuccessfully();
  }
}

ScAddr ASCPMathOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_math_operator;
}

ScAddr ASCPMathOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}

bool ASCPMathOperatorInterpreter::CheckInitiationCondition(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event)
{
  ScAddr const & scpOperatorAddr = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scpOperatorAddr, type))
    return false;
  return supportedOperators.count(type);
}

void ASCPMathOperatorInterpreter::InitializeSupportedOperators()
{
  supportedOperators = {
      {Keynodes::op_contSin,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorSIn(ctx, addr);
       }},
      {Keynodes::op_contCos,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorCos(ctx, addr);
       }},
      {Keynodes::op_contTg,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorTg(ctx, addr);
       }},
      {Keynodes::op_contASin,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorASin(ctx, addr);
       }},
      {Keynodes::op_contACos,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorACos(ctx, addr);
       }},
      {Keynodes::op_contATg,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorATg(ctx, addr);
       }},
      {Keynodes::op_contDivInt,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorDivInt(ctx, addr);
       }},
      {Keynodes::op_contDivRem,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorDivRem(ctx, addr);
       }},
      {Keynodes::op_ifEq,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorIfEq(ctx, addr);
       }},
      {Keynodes::op_contLn,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorContLn(ctx, addr);
       }},
      {Keynodes::op_ifGr,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorIfGr(ctx, addr);
       }},
      {Keynodes::op_contAdd,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorContAdd(ctx, addr);
       }},
      {Keynodes::op_contSub,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorContSub(ctx, addr);
       }},
      {Keynodes::op_contMult,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorContMult(ctx, addr);
       }},
      {Keynodes::op_contDiv,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorContDiv(ctx, addr);
       }},
      {Keynodes::op_contPow,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorContPow(ctx, addr);
       }},
  };
}

}  // namespace scp
