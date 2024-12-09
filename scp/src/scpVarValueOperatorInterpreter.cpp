/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpVarValueOperatorInterpreter.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "var_value_operators/SCPOperatorVarAssign.hpp"
#include "var_value_operators/SCPOperatorVarErase.hpp"

namespace scp
{
ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
    ASCPVarValueOperatorInterpreter::supportedOperators = {};

ScAddr ASCPVarValueOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_var_value_operator;
}

ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
ASCPVarValueOperatorInterpreter::getSupportedOperators() const
{
  if (supportedOperators.empty())
    supportedOperators = {
        {Keynodes::op_varAssign,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorVarAssign>(ctx, addr);
         }},
        {Keynodes::op_varErase,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorVarErase>(ctx, addr);
         }},
    };
  return supportedOperators;
}
}  // namespace scp
