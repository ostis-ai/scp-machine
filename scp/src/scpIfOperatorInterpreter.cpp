/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpIfOperatorInterpreter.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "if_operators/SCPOperatorIfCoin.hpp"
#include "if_operators/SCPOperatorIfType.hpp"
#include "if_operators/SCPOperatorIfVarAssign.hpp"

namespace scp
{
ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
    ASCPIfOperatorInterpreter::supportedOperators = {};

ScAddr ASCPIfOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_if_operator;
}

ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
ASCPIfOperatorInterpreter::getSupportedOperators() const
{
  if (supportedOperators.empty())
    supportedOperators = {
        {Keynodes::op_ifCoin,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorIfCoin>(ctx, addr);
         }},
        {Keynodes::op_ifType,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorIfType>(ctx, addr);
         }},
        {Keynodes::op_ifVarAssign,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorIfVarAssign>(ctx, addr);
         }},
    };
  return supportedOperators;
}
}  // namespace scp
