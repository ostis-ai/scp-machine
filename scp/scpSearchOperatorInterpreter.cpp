/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpSearchOperatorInterpreter.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "search_operators/SCPOperatorSearchElStr3.hpp"
#include "search_operators/SCPOperatorSearchElStr5.hpp"
#include "search_operators/SCPOperatorSearchSetStr3.hpp"
#include "search_operators/SCPOperatorSearchSetStr5.hpp"
#include "search_operators/SCPOperatorSysSearch.hpp"

namespace scp
{
ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
    ASCPSearchOperatorInterpreter::supportedOperators = {};

ScAddr ASCPSearchOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_search_operator;
}

ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
ASCPSearchOperatorInterpreter::getSupportedOperators() const
{
  if (supportedOperators.empty())
    supportedOperators = {
        {Keynodes::op_searchElStr3,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorSearchElStr3>(ctx, addr);
         }},
        {Keynodes::op_searchElStr5,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorSearchElStr5>(ctx, addr);
         }},
        {Keynodes::op_searchSetStr3,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorSearchSetStr3>(ctx, addr);
         }},
        {Keynodes::op_searchSetStr5,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorSearchSetStr5>(ctx, addr);
         }},
        {Keynodes::op_sys_search,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorSysSearch>(ctx, addr);
         }},
    };
  return supportedOperators;
}

}  // namespace scp
