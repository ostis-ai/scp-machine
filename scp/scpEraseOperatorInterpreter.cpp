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

namespace scp
{
ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
    ASCPEraseOperatorInterpreter::supportedOperators = {};

ScAddr ASCPEraseOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_erase_operator;
}

ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
ASCPEraseOperatorInterpreter::getSupportedOperators() const
{
  if (supportedOperators.empty())
    supportedOperators = {
        {Keynodes::op_eraseEl,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorEraseEl>(ctx, addr);
         }},
        {Keynodes::op_eraseElStr3,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorEraseElStr3>(ctx, addr);
         }},
        {Keynodes::op_eraseElStr5,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorEraseElStr5>(ctx, addr);
         }},
        {Keynodes::op_eraseSetStr3,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorEraseSetStr3>(ctx, addr);
         }},
        {Keynodes::op_eraseSetStr5,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorEraseSetStr5>(ctx, addr);
         }},
    };
  return supportedOperators;
}

}  // namespace scp
