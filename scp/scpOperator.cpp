/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-memory/sc_addr.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "scpOperator.hpp"

#include <algorithm>
#include <iostream>

namespace scp
{

SCPOperator::SCPOperator(ScMemoryContext & ctx_, ScAddr addr_)
  : addr(addr_)
  , m_memoryCtx(ctx_)
{
}

std::string SCPOperator::GetTypeName()
{
  return "unknown operator";
}

ScAddr SCPOperator::GetAddr()
{
  return addr;
}

sc_result SCPOperator::ResetValues()
{
  for (auto & operand : operands)
  {
    if (operand->IsAssign())
    {
      if (operand->IsSCPConst())
      {
#ifdef SCP_DEBUG
        Utils::logSCPError(m_memoryCtx, "SCP CONST must have FIXED modifier", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
      }
      operand->ResetValue();
    }
  }
  return SC_RESULT_OK;
}

sc_result SCPOperator::CheckNullValues()
{
  for (auto & operand : operands)
  {
    if (!operand)
    {
#ifdef SCP_DEBUG
      Utils::logSCPError(m_memoryCtx, "One or more operands missed", addr);
      Utils::printInfo(m_memoryCtx, addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }
  }
  return SC_RESULT_OK;
}

sc_result SCPOperator::Parse()
{
  std::fill(operands.begin(), operands.end(), nullptr);
  return SC_RESULT_OK;
}

sc_result SCPOperator::Execute()
{
  return SC_RESULT_OK;
}

void SCPOperator::ClearExecutionState()
{
  SCPOperator::ClearExecutionState(m_memoryCtx, addr);
}

void SCPOperator::FinishExecution()
{
  SCPOperator::FinishExecution(m_memoryCtx, addr);
}

void SCPOperator::FinishExecutionSuccessfully()
{
  SCPOperator::FinishExecutionSuccessfully(m_memoryCtx, addr);
}

void SCPOperator::FinishExecutionUnsuccessfully()
{
  SCPOperator::FinishExecutionUnsuccessfully(m_memoryCtx, addr);
}

void SCPOperator::FinishExecutionWithError()
{
  SCPOperator::FinishExecutionWithError(m_memoryCtx, addr);
}

void SCPOperator::ClearExecutionState(ScMemoryContext & ctx, ScAddr oper_addr)
{
  std::vector<ScAddr> arcs;
  ScIterator3Ptr iter = ctx.Iterator3(ScType::NodeConst, ScType::EdgeAccessConstPosPerm, oper_addr);
  while (iter->Next())
  {
    if (iter->Get(0) == Keynodes::active_action)
      arcs.push_back(iter->Get(1));
    if (iter->Get(0) == Keynodes::action_finished)
      arcs.push_back(iter->Get(1));
    if (iter->Get(0) == Keynodes::action_finished_successfully)
      arcs.push_back(iter->Get(1));
    if (iter->Get(0) == Keynodes::action_finished_unsuccessfully)
      arcs.push_back(iter->Get(1));
    if (iter->Get(0) == Keynodes::action_finished_with_error)
      arcs.push_back(iter->Get(1));
  }
  for (auto & arc : arcs)
    ctx.EraseElement(arc);
}

void SCPOperator::FinishExecution(ScMemoryContext & ctx, ScAddr oper_addr)
{
  ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::action_finished, oper_addr);
}

void SCPOperator::FinishExecutionSuccessfully(ScMemoryContext & ctx, ScAddr oper_addr)
{
  ClearExecutionState(ctx, oper_addr);
  ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::action_finished_successfully, oper_addr);
  FinishExecution(ctx, oper_addr);
}

void SCPOperator::FinishExecutionUnsuccessfully(ScMemoryContext & ctx, ScAddr oper_addr)
{
  ClearExecutionState(ctx, oper_addr);
  ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::action_finished_unsuccessfully, oper_addr);
  FinishExecution(ctx, oper_addr);
}

void SCPOperator::FinishExecutionWithError(ScMemoryContext & ctx, ScAddr oper_addr)
{
  ClearExecutionState(ctx, oper_addr);
  ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::action_finished_with_error, oper_addr);
  FinishExecution(ctx, oper_addr);
}

SCPOperator::~SCPOperator()
{
  for (auto & operand : operands)
  {
    delete operand;
  }
}

}  // namespace scp
