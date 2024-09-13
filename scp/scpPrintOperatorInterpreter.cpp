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
ScAddrToValueUnorderedMap<std::function<SCPOperator *(ScAgentContext &, ScAddr)>>
    ASCPPrintOperatorInterpreter::supportedOperators = {};

ScResult ASCPPrintOperatorInterpreter::DoProgram(
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
  ScAddr const & scpOperatorAddr = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scpOperatorAddr, type))
    return false;
  return supportedOperators.count(type);
}

void ASCPPrintOperatorInterpreter::InitializeSupportedOperators()
{
  supportedOperators = {
      {Keynodes::op_printEl,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorPrintEl(ctx, addr);
       }},
      {Keynodes::op_print,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorPrint(ctx, addr, false);
       }},
      {Keynodes::op_printNl,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorPrint(ctx, addr, true);
       }},
  };
}
}  // namespace scp
