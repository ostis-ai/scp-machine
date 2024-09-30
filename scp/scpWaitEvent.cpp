/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-memory/sc_addr.hpp"
#include "scp.hpp"
#include "scpWaitEvent.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include <iostream>

namespace scp
{

concurrent_deque<SCPWaitEvent *> SCPWaitEvent::sys_wait_events;

void SCPWaitEvent::DeleteAllSysWaiters()
{
  while (!sys_wait_events.empty())
  {
    SCPWaitEvent * event = sys_wait_events.front();
    sys_wait_events.pop();
    delete event;
  }
}

SCPWaitEvent::SCPWaitEvent(
    ScAgentContext & ctx,
    ScAddr const & subscribedElement,
    ScAddr const & eventType,
    ScAddr const & waitingOperator)
  : paramAddr(waitingOperator)
{
  SC_LOG_INFO("Creating SCPWaitEvent with event type " << ctx.GetElementSystemIdentifier(eventType));
  m_event = ctx.CreateElementaryEventSubscription(
      eventType,
      subscribedElement,
      [waitingOperator](ScElementaryEvent const & scEvent)
      {
        if (!ScMemory::ms_globalContext->CheckConnector(
                Keynodes::action_finished_successfully, waitingOperator, ScType::EdgeAccessConstPosPerm))
          ScMemory::ms_globalContext->GenerateConnector(
              ScType::EdgeAccessConstPosPerm, Keynodes::action_finished_successfully, waitingOperator);
        else
          return;
        if (!ScMemory::ms_globalContext->CheckConnector(
                Keynodes::action_finished, waitingOperator, ScType::EdgeAccessConstPosPerm))
          ScMemory::ms_globalContext->GenerateConnector(
              ScType::EdgeAccessConstPosPerm, Keynodes::action_finished, waitingOperator);
        else
          return;

        auto checker = [&waitingOperator](SCPWaitEvent * event)
        {
          return event->paramAddr == waitingOperator;
        };

        SCPWaitEvent * event;
        if (sys_wait_events.extract(checker, event))
          delete event;
      });
}

}  // namespace scp
