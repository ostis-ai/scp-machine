/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent_context.hpp>
#include "scpKeynodes.hpp"
#include "scpOperand.hpp"

#include <vector>

namespace scp
{

class SCPOperator
{
protected:
  ScAddr addr;
  ScAddr type;
  std::vector<SCPOperand *> operands;

  ScAgentContext & m_memoryCtx;

  sc_result ResetValues();
  sc_result CheckNullValues();

public:
  SCPOperator(ScAgentContext & ctx_, ScAddr addr_);
  virtual ~SCPOperator();
  ScAddr GetAddr();
  virtual std::string GetTypeName();
  virtual sc_result Parse();
  virtual sc_result Execute();
  void ClearExecutionState();
  void FinishExecution();
  void FinishExecutionSuccessfully();
  void FinishExecutionUnsuccessfully();
  void FinishExecutionWithError();
  static void ClearExecutionState(ScAgentContext & ctx, ScAddr oper_addr, ScAddrUnorderedSet const & ignoredStates = {});
  static void FinishExecution(ScAgentContext & ctx, ScAddr oper_addr);
  static void FinishExecutionSuccessfully(ScAgentContext & ctx, ScAddr oper_addr);
  static void FinishExecutionUnsuccessfully(ScAgentContext & ctx, ScAddr oper_addr);
  static void FinishExecutionWithError(ScAgentContext & ctx, ScAddr oper_addr);
};
}  // namespace scp
