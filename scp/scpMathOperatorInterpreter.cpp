#include "scpMathOperatorInterpreter.hpp"

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "math_operators/SCPOperatorSin.hpp"
#include "math_operators/SCPOperatorCos.hpp"
#include "math_operators/SCPOperatorTg.hpp"
#include "math_operators/SCPOperatorASin.hpp"
#include "math_operators/SCPOperatorACos.hpp"
#include "math_operators/SCPOperatorATg.hpp"
#include "math_operators/SCPOperatorDivInt.hpp"
#include "math_operators/SCPOperatorDivRem.hpp"
#include "math_operators/SCPOperatorIfEq.hpp"
#include "math_operators/SCPOperatorIfGr.hpp"
#include "math_operators/SCPOperatorContLn.hpp"
#include "math_operators/SCPOperatorContAdd.hpp"
#include "math_operators/SCPOperatorContSub.hpp"
#include "math_operators/SCPOperatorContMult.hpp"
#include "math_operators/SCPOperatorContPow.hpp"
#include "math_operators/SCPOperatorContDiv.hpp"

#include <iostream>
#include <string>

namespace scp
{
ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
    ASCPMathOperatorInterpreter::supportedOperators = {};

ScAddr ASCPMathOperatorInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_math_operator;
}

ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
ASCPMathOperatorInterpreter::getSupportedOperators() const
{
  if (supportedOperators.empty())
    supportedOperators = {
        {Keynodes::op_contSin,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorSIn>(ctx, addr);
         }},
        {Keynodes::op_contCos,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorCos>(ctx, addr);
         }},
        {Keynodes::op_contTg,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorTg>(ctx, addr);
         }},
        {Keynodes::op_contASin,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorASin>(ctx, addr);
         }},
        {Keynodes::op_contACos,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorACos>(ctx, addr);
         }},
        {Keynodes::op_contATg,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorATg>(ctx, addr);
         }},
        {Keynodes::op_contDivInt,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorDivInt>(ctx, addr);
         }},
        {Keynodes::op_contDivRem,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorDivRem>(ctx, addr);
         }},
        {Keynodes::op_ifEq,  // todo(kilativ-dotcom): move to if operators
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorIfEq>(ctx, addr);
         }},
        {Keynodes::op_contLn,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorContLn>(ctx, addr);
         }},
        {Keynodes::op_ifGr,  // todo(kilativ-dotcom): move to if operators
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorIfGr>(ctx, addr);
         }},
        {Keynodes::op_contAdd,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorContAdd>(ctx, addr);
         }},
        {Keynodes::op_contSub,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorContSub>(ctx, addr);
         }},
        {Keynodes::op_contMult,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorContMult>(ctx, addr);
         }},
        {Keynodes::op_contDiv,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorContDiv>(ctx, addr);
         }},
        {Keynodes::op_contPow,
         [](ScAgentContext & ctx, ScAddr addr)
         {
           return std::make_unique<SCPOperatorContPow>(ctx, addr);
         }},
    };
  return supportedOperators;
}

}  // namespace scp
