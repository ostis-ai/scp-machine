/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "scpKeynodes.hpp"
#include "scpOperand.hpp"
#include "scpOperator.hpp"

#include <vector>

namespace scp
{

class SCPOperatorSetStr3 : public SCPOperator
{
protected:
  std::vector<SCPOperand *> set_operands;

public:
  SCPOperatorSetStr3(ScAgentContext & ctx, ScAddr addr);
  sc_result Parse();
  ~SCPOperatorSetStr3();
};

class SCPOperatorSetStr5 : public SCPOperator
{
protected:
  std::vector<SCPOperand *> set_operands;

public:
  SCPOperatorSetStr5(ScAgentContext & ctx, ScAddr addr);
  sc_result Parse();
  ~SCPOperatorSetStr5();
};

}  // namespace scp
