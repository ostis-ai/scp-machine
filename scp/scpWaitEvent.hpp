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
  sc_addr static resolve_sc_addr_from_pointer(sc_pointer data);
  static sc_result Run(sc_event_subscription const * evt, sc_addr edge);
  sc_event_subscription * m_event;

public:
  static concurrent_deque<SCPWaitEvent *> sys_wait_events;
  void static unregister_all_sys_wait();
  SCPWaitEvent(ScMemoryContext & ctx, ScAddr const & addr, ScAddr const & eventType, ScAddr const & param_addr);
  ScAddr GetParamAddr();
  ~SCPWaitEvent();
};

}  // namespace scp
