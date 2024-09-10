/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpPrintOperatorInterpreter.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "print_operators/SCPOperatorPrintEl.hpp"
#include "print_operators/SCPOperatorPrint.hpp"

#include <iostream>

namespace scp
{
ScAddrToValueUnorderedMap<std::function<SCPOperator *(ScMemoryContext &, ScAddr)>>
    ASCPPrintOperatorInterpreter::supportedOperators = {};

ScResult ASCPPrintOperatorInterpreter::DoProgram(
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

ScAddr ASCPPrintOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_print_operator;
}

ScAddr ASCPPrintOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}

bool ASCPPrintOperatorInterpreter::CheckInitiationCondition(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event)
{
  ScAddr scp_operator = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scp_operator, type))
    return false;
  return supportedOperators.count(type);
}

void ASCPPrintOperatorInterpreter::InitializeSupportedOperators()
{
  supportedOperators = {
      {Keynodes::op_printEl,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorPrintEl(ctx, addr);
       }},
      {Keynodes::op_print,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorPrint(ctx, addr, false);
       }},
      {Keynodes::op_printNl,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorPrint(ctx, addr, true);
       }},
  };
}
}  // namespace scp
