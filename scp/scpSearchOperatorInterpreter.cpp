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
ScAddrToValueUnorderedMap<std::function<SCPOperator *(ScMemoryContext &, ScAddr)>>
    ASCPSearchOperatorInterpreter::supportedOperators = {};

ScResult ASCPSearchOperatorInterpreter::DoProgram(
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

  auto const & pair = supportedOperators.find(type);
  if (pair != supportedOperators.cend())
    oper = pair->second(m_context, scp_operator);

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
  ScAddr scp_operator = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scp_operator, type))
    return false;
  auto const & pair = supportedOperators.find(type);
  return pair != supportedOperators.cend();
}

void ASCPSearchOperatorInterpreter::InitializeSupportedOperators()
{
  supportedOperators = {
      {Keynodes::op_searchElStr3,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorSearchElStr3(ctx, addr);
       }},
      {Keynodes::op_searchElStr5,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorSearchElStr5(ctx, addr);
       }},
      {Keynodes::op_searchSetStr3,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorSearchSetStr3(ctx, addr);
       }},
      {Keynodes::op_searchSetStr5,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorSearchSetStr5(ctx, addr);
       }},
      {Keynodes::op_sys_search,
       [](ScMemoryContext & ctx, ScAddr addr)
       {
         return new SCPOperatorSysSearch(ctx, addr);
       }},
  };
}

}  // namespace scp
