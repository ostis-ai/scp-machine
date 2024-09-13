/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpGenOperatorInterpreter.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "gen_operators/SCPOperatorGenEl.hpp"
#include "gen_operators/SCPOperatorGenElStr3.hpp"
#include "gen_operators/SCPOperatorGenElStr5.hpp"
#include "gen_operators/SCPOperatorSysGen.hpp"

#include <iostream>

namespace scp
{
ScAddrToValueUnorderedMap<std::function<SCPOperator *(ScAgentContext &, ScAddr)>>
    ASCPGenOperatorInterpreter::supportedOperators = {};

ScResult ASCPGenOperatorInterpreter::DoProgram(
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

ScAddr ASCPGenOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_gen_operator;
}

ScAddr ASCPGenOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}

bool ASCPGenOperatorInterpreter::CheckInitiationCondition(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event)
{
  ScAddr const & scpOperatorAddr = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scpOperatorAddr, type))
    return false;
  auto const & pair = supportedOperators.find(type);
  return pair != supportedOperators.cend();
}

void ASCPGenOperatorInterpreter::InitializeSupportedOperators()
{
  supportedOperators = {
      {Keynodes::op_genEl,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorGenEl(ctx, addr);
       }},
      {Keynodes::op_genElStr3,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorGenElStr3(ctx, addr);
       }},
      {Keynodes::op_genElStr5,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorGenElStr5(ctx, addr);
       }},
      {Keynodes::op_sys_gen,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorSysGen(ctx, addr);
       }},
  };
}

}  // namespace scp
