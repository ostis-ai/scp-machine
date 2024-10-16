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

concurrent_deque<SCPWaitEvent *> SCPWaitEvent::sysWaitEventSubscriptions;

void SCPWaitEvent::DeleteAllSysWaiters()
{
  while (!sysWaitEventSubscriptions.empty())
  {
    SCPWaitEvent * event = sysWaitEventSubscriptions.front();
    sysWaitEventSubscriptions.pop();
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
  m_eventSubscription = ctx.CreateElementaryEventSubscription(
      eventType,
      subscribedElement,
      [waitingOperator](ScElementaryEvent const & scEvent)
      {
        if (!ScMemory::ms_globalContext->CheckConnector(
                Keynodes::action_finished_successfully, waitingOperator, ScType::ConstPermPosArc))
          ScMemory::ms_globalContext->GenerateConnector(
              ScType::ConstPermPosArc, Keynodes::action_finished_successfully, waitingOperator);
        else
          return;
        if (!ScMemory::ms_globalContext->CheckConnector(
                Keynodes::action_finished, waitingOperator, ScType::ConstPermPosArc))
          ScMemory::ms_globalContext->GenerateConnector(
              ScType::ConstPermPosArc, Keynodes::action_finished, waitingOperator);
        else
          return;

        auto checker = [&waitingOperator](SCPWaitEvent * event)
        {
          return event->paramAddr == waitingOperator;
        };

        SCPWaitEvent * eventSubscription;
        if (sysWaitEventSubscriptions.extract(checker, eventSubscription))
          delete eventSubscription;
      });
}

}  // namespace scp
