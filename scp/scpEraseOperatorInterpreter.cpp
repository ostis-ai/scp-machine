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
#include "sc-memory/sc_memory.hpp"
#include <iostream>

namespace scp
{

ScResult
ASCPEraseOperatorInterpreter::DoProgram(ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event,
                                        ScAction & action)
{
  if (!event.GetArc().IsValid())
    return action.FinishUnsuccessfully();

  ScAddr scp_operator = event.GetOtherElement();

  ScAddr type;
  if (SC_TRUE != Utils::resolveOperatorType(m_context, scp_operator, type))
    return action.FinishUnsuccessfully();

  SCPOperator * oper = nullptr;
  if (type == Keynodes::op_eraseEl)
  {
    oper = new SCPOperatorEraseEl(m_context, scp_operator);
  }
  if (type == Keynodes::op_eraseElStr3)
  {
    oper = new SCPOperatorEraseElStr3(m_context, scp_operator);
  }
  if (type == Keynodes::op_eraseElStr5)
  {
    oper = new SCPOperatorEraseElStr5(m_context, scp_operator);
  }
  if (type == Keynodes::op_eraseSetStr3)
  {
    oper = new SCPOperatorEraseSetStr3(m_context, scp_operator);
  }
  if (type == Keynodes::op_eraseSetStr5)
  {
    oper = new SCPOperatorEraseSetStr5(m_context, scp_operator);
  }
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
  } else
  {
    sc_result execute_result;
    execute_result = oper->Execute();
    delete oper;
    return (execute_result == SC_RESULT_OK) ? action.FinishSuccessfully() : action.FinishUnsuccessfully();
  }
}

ScAddr ASCPEraseOperatorInterpreter::GetActionClass() const
{
//todo(codegen-removal): replace action with your action class
  return ScKeynodes::action;
}

ScAddr ASCPEraseOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}

}
