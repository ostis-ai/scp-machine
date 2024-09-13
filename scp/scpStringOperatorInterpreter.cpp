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
ScAddrToValueUnorderedMap<std::function<SCPOperator *(ScAgentContext &, ScAddr)>>
    ASCPStringOperatorInterpreter::supportedOperators = {};

ScResult ASCPStringOperatorInterpreter::DoProgram(
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

#ifdef SCP_DEBUG
  std::cout << scpOperator->GetTypeName() << std::endl;
#endif

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

ScAddr ASCPStringOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_string_operator;
}

ScAddr ASCPStringOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}

bool ASCPStringOperatorInterpreter::CheckInitiationCondition(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event)
{
  ScAddr const & scpOperatorAddr = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scpOperatorAddr, type))
    return false;
  return supportedOperators.count(type);
}

void ASCPStringOperatorInterpreter::InitializeSupportedOperators()
{
  supportedOperators = {
      {Keynodes::op_stringSplit,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorStringSplit(ctx, addr);
       }},
      {Keynodes::op_stringSlice,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorStringSlice(ctx, addr);
       }},
      {Keynodes::op_stringReplace,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorStringReplace(ctx, addr);
       }},
      {Keynodes::op_contStringConcat,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorContStringConcat(ctx, addr);
       }},
      {Keynodes::op_stringIfEq,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorStringIfEq(ctx, addr);
       }},
      {Keynodes::op_stringIfGr,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorStringIfGr(ctx, addr);
       }},
      {Keynodes::op_stringLen,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorStringLen(ctx, addr);
       }},
      {Keynodes::op_stringSub,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorStringSub(ctx, addr);
       }},
      {Keynodes::op_stringStartsWith,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorStringStartsWith(ctx, addr);
       }},
      {Keynodes::op_stringEndsWith,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorStringEndsWith(ctx, addr);
       }},
      {Keynodes::op_stringToUpperCase,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorStringToUpperCase(ctx, addr);
       }},
      {Keynodes::op_stringToLowerCase,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorStringToLowerCase(ctx, addr);
       }},
  };
}

}  // namespace scp
