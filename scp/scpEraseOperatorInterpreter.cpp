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
ScAddrToValueUnorderedMap<std::function<SCPOperator *(ScAgentContext &, ScAddr)>>
    ASCPEraseOperatorInterpreter::supportedOperators = {};

ScResult ASCPEraseOperatorInterpreter::DoProgram(
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
  ScAddr const & scpOperatorAddr = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scpOperatorAddr, type))
    return false;
  return supportedOperators.count(type);
}

void ASCPEraseOperatorInterpreter::InitializeSupportedOperators()
{
  ASCPEraseOperatorInterpreter::supportedOperators = {
      {Keynodes::op_eraseEl,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorEraseEl(ctx, addr);
       }},
      {Keynodes::op_eraseElStr3,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorEraseElStr3(ctx, addr);
       }},
      {Keynodes::op_eraseElStr5,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorEraseElStr5(ctx, addr);
       }},
      {Keynodes::op_eraseSetStr3,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorEraseSetStr3(ctx, addr);
       }},
      {Keynodes::op_eraseSetStr5,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorEraseSetStr5(ctx, addr);
       }},
  };
}

}  // namespace scp
