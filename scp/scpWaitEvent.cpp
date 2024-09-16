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

extern "C"
{
#include "sc-core/sc-store/sc_event_subscription.h"
}

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
    ScAddr const & addr,
    ScAddr const & eventType,
    ScAddr const & paramAddr)
  : paramAddr(paramAddr)
{
  m_event = ctx.CreateElementaryEventSubscription(
      eventType,
      addr,
      [&ctx, &paramAddr](ScElementaryEvent const & scEvent)
      {
        ctx.GenerateConnector(ScType::EdgeAccessConstPosPerm, Keynodes::action_finished_successfully, paramAddr);
        ctx.GenerateConnector(ScType::EdgeAccessConstPosPerm, Keynodes::action_finished, paramAddr);

        auto checker = [&paramAddr](SCPWaitEvent * event)
        {
          return event->paramAddr == paramAddr;
        };

        SCPWaitEvent * event;
        if (sys_wait_events.extract(checker, event))
        {
          delete event;
        }
      });
}

}  // namespace scp
