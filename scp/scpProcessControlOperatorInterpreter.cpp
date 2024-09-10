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

namespace scp
{
ScAddrToValueUnorderedMap<std::function<SCPOperator *(ScMemoryContext &, ScAddr)>>
    ASCPProcessControlOperatorInterpreter::supportedOperators = {};

ScResult ASCPProcessControlOperatorInterpreter::DoProgram(
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

ScAddr ASCPProcessControlOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_process_control_operator;
}

ScAddr ASCPProcessControlOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}

bool ASCPProcessControlOperatorInterpreter::CheckInitiationCondition(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event)
{
  ScAddr scp_operator = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scp_operator, type))
    return false;
  return supportedOperators.count(type);
}

void ASCPProcessControlOperatorInterpreter::InitializeSupportedOperators()
{
  supportedOperators = {
      {Keynodes::op_return,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorReturn(ctx, addr);
       }},
      {Keynodes::op_sys_wait,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorSysWait(ctx, addr);
       }},
      {Keynodes::op_call,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorCall(ctx, addr);
       }},
      {Keynodes::op_waitReturn,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorWaitReturn(ctx, addr);
       }},
  };
}

}  // namespace scp
