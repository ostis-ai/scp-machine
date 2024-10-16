/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpStringOperatorInterpreter.hpp"

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "string_operators/SCPOperatorStringSplit.hpp"
#include "string_operators/SCPOperatorStringReplace.hpp"
#include "string_operators/SCPOperatorStringSlice.hpp"
#include "string_operators/SCPOperatorContStringConcat.hpp"
#include "string_operators/SCPOperatorStringStartsWith.hpp"
#include "string_operators/SCPOperatorStringEndsWith.hpp"
#include "string_operators/SCPOperatorStringToLowerCase.hpp"
#include "string_operators/SCPOperatorStringToUpperCase.hpp"
#include "string_operators/SCPOperatorStringLen.hpp"
#include "string_operators/SCPOperatorStringSub.hpp"
#include "string_operators/SCPOperatorStringIfEq.hpp"
#include "string_operators/SCPOperatorStringIfGr.hpp"

namespace scp
{
ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
    ASCPStringOperatorInterpreter::supportedOperators = {};

ScAddr ASCPStringOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_string_operator;
}

ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
ASCPStringOperatorInterpreter::getSupportedOperators() const
{
  if (supportedOperators.empty())
    supportedOperators = {
        {Keynodes::op_stringSplit,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorStringSplit>(ctx, addr);
         }},
        {Keynodes::op_stringSlice,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorStringSlice>(ctx, addr);
         }},
        {Keynodes::op_stringReplace,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorStringReplace>(ctx, addr);
         }},
        {Keynodes::op_contStringConcat,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorContStringConcat>(ctx, addr);
         }},
        {Keynodes::op_stringIfEq,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorStringIfEq>(ctx, addr);
         }},
        {Keynodes::op_stringIfGr,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorStringIfGr>(ctx, addr);
         }},
        {Keynodes::op_stringLen,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorStringLen>(ctx, addr);
         }},
        {Keynodes::op_stringSub,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorStringSub>(ctx, addr);
         }},
        {Keynodes::op_stringStartsWith,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorStringStartsWith>(ctx, addr);
         }},
        {Keynodes::op_stringEndsWith,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorStringEndsWith>(ctx, addr);
         }},
        {Keynodes::op_stringToUpperCase,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorStringToUpperCase>(ctx, addr);
         }},
        {Keynodes::op_stringToLowerCase,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorStringToLowerCase>(ctx, addr);
         }},
    };
  return supportedOperators;
}

}  // namespace scp
