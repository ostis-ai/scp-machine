/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpProcessControlOperatorInterpreter.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "process_control_operators/SCPOperatorReturn.hpp"
#include "process_control_operators/SCPOperatorSysWait.hpp"
#include "process_control_operators/SCPOperatorCall.hpp"
#include "process_control_operators/SCPOperatorWaitReturn.hpp"

namespace scp
{
ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
    ASCPProcessControlOperatorInterpreter::supportedOperators = {};

ScAddr ASCPProcessControlOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_process_control_operator;
}

ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
ASCPProcessControlOperatorInterpreter::getSupportedOperators() const
{
  if (supportedOperators.empty())
    supportedOperators = {
        {Keynodes::op_return,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorReturn>(ctx, addr);
         }},
        {Keynodes::op_sys_wait,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorSysWait>(ctx, addr);
         }},
        {Keynodes::op_call,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorCall>(ctx, addr);
         }},
        {Keynodes::op_waitReturn,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorWaitReturn>(ctx, addr);
         }},
    };
  return supportedOperators;
}

}  // namespace scp
