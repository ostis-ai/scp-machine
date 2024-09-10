/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "scpEraseOperatorInterpreter.hpp"
#include "erase_operators/SCPOperatorEraseEl.hpp"
#include "erase_operators/SCPOperatorEraseElStr3.hpp"
#include "erase_operators/SCPOperatorEraseElStr5.hpp"
#include "erase_operators/SCPOperatorEraseSetStr3.hpp"
#include "erase_operators/SCPOperatorEraseSetStr5.hpp"

#include <iostream>

namespace scp
{
ScAddrToValueUnorderedMap<std::function<SCPOperator *(ScMemoryContext &, ScAddr)>>
    ASCPEraseOperatorInterpreter::supportedOperators = {};

ScResult ASCPEraseOperatorInterpreter::DoProgram(
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

ScAddr ASCPEraseOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_erase_operator;
}

ScAddr ASCPEraseOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}

bool ASCPEraseOperatorInterpreter::CheckInitiationCondition(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event)
{
  ScAddr scp_operator = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scp_operator, type))
    return false;
  return supportedOperators.count(type);
}

void ASCPEraseOperatorInterpreter::InitializeSupportedOperators()
{
  ASCPEraseOperatorInterpreter::supportedOperators = {
      {Keynodes::op_eraseEl,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorEraseEl(ctx, addr);
       }},
      {Keynodes::op_eraseElStr3,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorEraseElStr3(ctx, addr);
       }},
      {Keynodes::op_eraseElStr5,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorEraseElStr5(ctx, addr);
       }},
      {Keynodes::op_eraseSetStr3,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorEraseSetStr3(ctx, addr);
       }},
      {Keynodes::op_eraseSetStr5,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorEraseSetStr5(ctx, addr);
       }},
  };
}

}  // namespace scp
