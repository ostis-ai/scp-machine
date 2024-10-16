/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "SCPOperatorSysWait.hpp"
#include "scpWaitEvent.hpp"

#include <iostream>

namespace scp
{

ScAddr SCPOperatorSysWait::resolve_event_type(ScAddr const & event_type_node)
{
  if (event_type_node == Keynodes::sc_event_after_generate_incoming_arc)
    return event_type_node;
  if (event_type_node == Keynodes::sc_event_after_generate_outgoing_arc)
    return event_type_node;

  if (event_type_node == Keynodes::sc_event_before_erase_outgoing_arc)
    return event_type_node;
  if (event_type_node == Keynodes::sc_event_before_erase_incoming_arc)
    return event_type_node;

  if (event_type_node == Keynodes::sc_event_before_change_link_content)
    return event_type_node;

  if (event_type_node == Keynodes::sc_event_before_erase_element)
    return event_type_node;

  return Keynodes::sc_event_after_generate_incoming_arc;
}

SCPOperatorSysWait::SCPOperatorSysWait(ScAgentContext & ctx, ScAddr addr)
  : SCPOperatorElStr2(ctx, addr)
{
}

std::string SCPOperatorSysWait::GetTypeName()
{
  return "sys_wait";
}

sc_result SCPOperatorSysWait::Parse()
{
  return SCPOperatorElStr2::Parse();
}

sc_result SCPOperatorSysWait::Execute()
{
  ClearExecutionState(m_memoryCtx, addr, {Keynodes::active_action});
  if (SC_RESULT_OK != ResetValues())
    return SC_RESULT_ERROR;

  if (!(operands[0]->IsFixed() && operands[1]->IsFixed()))
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Both operands must have FIXED modifier", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }
  if (!operands[0]->GetValue().IsValid())
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand 1 has modifier FIXED, but has no value", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  if (!operands[1]->GetValue().IsValid())
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand 2 has modifier FIXED, but has no value", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  auto * event =
      new SCPWaitEvent(m_memoryCtx, operands[1]->GetValue(), resolve_event_type(operands[0]->GetValue()), addr);
  SCPWaitEvent::sysWaitEventSubscriptions.push(event);

  return SC_RESULT_OK;
}
}  // namespace scp
