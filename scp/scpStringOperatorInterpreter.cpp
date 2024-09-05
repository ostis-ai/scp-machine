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
ScAddrToValueUnorderedMap<std::function<SCPOperator *(ScMemoryContext &, ScAddr)>>
    ASCPStringOperatorInterpreter::supportedOperators = {
        {Keynodes::op_stringSplit,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorStringSplit(ctx, addr);
         }},
        {Keynodes::op_stringSlice,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorStringSlice(ctx, addr);
         }},
        {Keynodes::op_stringReplace,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorStringReplace(ctx, addr);
         }},
        {Keynodes::op_contStringConcat,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorContStringConcat(ctx, addr);
         }},
        {Keynodes::op_stringIfEq,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorStringIfEq(ctx, addr);
         }},
        {Keynodes::op_stringIfGr,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorStringIfGr(ctx, addr);
         }},
        {Keynodes::op_stringLen,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorStringLen(ctx, addr);
         }},
        {Keynodes::op_stringSub,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorStringSub(ctx, addr);
         }},
        {Keynodes::op_stringStartsWith,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorStringStartsWith(ctx, addr);
         }},
        {Keynodes::op_stringEndsWith,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorStringEndsWith(ctx, addr);
         }},
        {Keynodes::op_stringToUpperCase,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorStringToUpperCase(ctx, addr);
         }},
        {Keynodes::op_stringToLowerCase,
         [](ScMemoryContext & ctx, ScAddr addr)
         {
           return new SCPOperatorStringToLowerCase(ctx, addr);
         }},
};

ScResult ASCPStringOperatorInterpreter::DoProgram(
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
  // todo(codegen-removal): replace action with your action class
  return ScKeynodes::action;
}

ScAddr ASCPStringOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}

bool ASCPStringOperatorInterpreter::CheckInitiationCondition(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event)
{
  ScAddr scp_operator = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scp_operator, type))
    return false;
  return supportedOperators.count(type);
}

}  // namespace scp
