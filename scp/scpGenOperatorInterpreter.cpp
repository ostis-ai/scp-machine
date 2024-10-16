/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpGenOperatorInterpreter.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "gen_operators/SCPOperatorGenEl.hpp"
#include "gen_operators/SCPOperatorGenElStr3.hpp"
#include "gen_operators/SCPOperatorGenElStr5.hpp"
#include "gen_operators/SCPOperatorSysGen.hpp"

namespace scp
{
ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
    ASCPGenOperatorInterpreter::supportedOperators = {};

ScAddr ASCPGenOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_gen_operator;
}

ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
ASCPGenOperatorInterpreter::getSupportedOperators() const
{
  if (supportedOperators.empty())
    supportedOperators = {
        {Keynodes::op_genEl,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorGenEl>(ctx, addr);
         }},
        {Keynodes::op_genElStr3,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorGenElStr3>(ctx, addr);
         }},
        {Keynodes::op_genElStr5,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorGenElStr5>(ctx, addr);
         }},
        {Keynodes::op_sys_gen,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorSysGen>(ctx, addr);
         }},
    };
  return supportedOperators;
}

}  // namespace scp
