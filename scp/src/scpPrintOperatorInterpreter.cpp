/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpPrintOperatorInterpreter.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "print_operators/SCPOperatorPrintEl.hpp"
#include "print_operators/SCPOperatorPrint.hpp"

namespace scp
{
ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
    ASCPPrintOperatorInterpreter::supportedOperators = {};

ScAddr ASCPPrintOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_print_operator;
}

ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
ASCPPrintOperatorInterpreter::getSupportedOperators() const
{
  if (supportedOperators.empty())
    supportedOperators = {
        {Keynodes::op_printEl,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorPrintEl>(ctx, addr);
         }},
        {Keynodes::op_print,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorPrint>(ctx, addr, false);
         }},
        {Keynodes::op_printNl,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorPrint>(ctx, addr, true);
         }},
    };
  return supportedOperators;
}
}  // namespace scp
