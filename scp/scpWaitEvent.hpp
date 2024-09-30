/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_event.hpp"
#include "concurrent_queue.hpp"

namespace scp
{

class SCPWaitEvent
{
private:
  std::shared_ptr<ScElementaryEventSubscription<ScElementaryEvent>> m_event;
  ScAddr const & paramAddr;

public:
  static concurrent_deque<SCPWaitEvent *> sys_wait_events;
  void static DeleteAllSysWaiters();
  SCPWaitEvent(
      ScAgentContext & ctx,
      ScAddr const & subscribedElement,
      ScAddr const & eventType,
      ScAddr const & waitingOperator);
};

}  // namespace scp
