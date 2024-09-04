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
    if (event_type_node == Keynodes::sc_event_add_output_arc)
        return event_type_node;
    if (event_type_node == Keynodes::sc_event_add_input_arc)
        return event_type_node;

    if (event_type_node == Keynodes::sc_event_remove_output_arc)
        return event_type_node;
    if (event_type_node == Keynodes::sc_event_remove_input_arc)
        return event_type_node;

    if (event_type_node == Keynodes::sc_event_content_changed)
        return event_type_node;

    if (event_type_node == Keynodes::sc_event_remove_element)
        return event_type_node;

    return Keynodes::sc_event_add_output_arc;
}

SCPOperatorSysWait::SCPOperatorSysWait(ScMemoryContext &ctx, ScAddr addr): SCPOperatorElStr2(ctx, addr)
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

    auto * event = new SCPWaitEvent(m_memoryCtx, operands[1]->GetValue(), resolve_event_type(operands[0]->GetValue()), addr);
    SCPWaitEvent::sys_wait_events.push(event);

    return SC_RESULT_OK;
}
}
