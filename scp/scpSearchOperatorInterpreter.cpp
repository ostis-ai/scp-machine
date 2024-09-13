/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpSearchOperatorInterpreter.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "search_operators/SCPOperatorSearchElStr3.hpp"
#include "search_operators/SCPOperatorSearchElStr5.hpp"
#include "search_operators/SCPOperatorSearchSetStr3.hpp"
#include "search_operators/SCPOperatorSearchSetStr5.hpp"
#include "search_operators/SCPOperatorSysSearch.hpp"

#include <iostream>

namespace scp
{
ScAddrToValueUnorderedMap<std::function<SCPOperator *(ScAgentContext &, ScAddr)>>
    ASCPSearchOperatorInterpreter::supportedOperators = {};

ScResult ASCPSearchOperatorInterpreter::DoProgram(
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

ScAddr ASCPSearchOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_search_operator;
}

ScAddr ASCPSearchOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}

bool ASCPSearchOperatorInterpreter::CheckInitiationCondition(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event)
{
  ScAddr const & scpOperatorAddr = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scpOperatorAddr, type))
    return false;
  auto const & pair = supportedOperators.find(type);
  return pair != supportedOperators.cend();
}

void ASCPSearchOperatorInterpreter::InitializeSupportedOperators()
{
  supportedOperators = {
      {Keynodes::op_searchElStr3,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorSearchElStr3(ctx, addr);
       }},
      {Keynodes::op_searchElStr5,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorSearchElStr5(ctx, addr);
       }},
      {Keynodes::op_searchSetStr3,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorSearchSetStr3(ctx, addr);
       }},
      {Keynodes::op_searchSetStr5,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorSearchSetStr5(ctx, addr);
       }},
      {Keynodes::op_sys_search,
       [](ScAgentContext & ctx, ScAddr addr)
       {
         return new SCPOperatorSysSearch(ctx, addr);
       }},
  };
}

}  // namespace scp
