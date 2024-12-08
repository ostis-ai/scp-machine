/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "SCPOperatorIfTypeCoin.hpp"
#include "sc-memory/sc_memory.hpp"
#include <iostream>

namespace scp
{

SCPOperatorIfTypeCoin::SCPOperatorIfTypeCoin(ScAgentContext & ctx, ScAddr addr)
  : SCPOperatorElStr2(ctx, addr)
{
}

std::string SCPOperatorIfTypeCoin::GetTypeName()
{
  return "ifTypeCoin";
}

sc_result SCPOperatorIfTypeCoin::Parse()
{
  return SCPOperatorElStr2::Parse();
}

sc_result SCPOperatorIfTypeCoin::Execute()
{
  if (SC_RESULT_OK != ResetValues())
    return SC_RESULT_ERROR;

  if (!(operands[0]->IsFixed() && operands[1]->IsFixed()))
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Both operands must have FIXED modifier", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }
  if (!operands[0]->GetValue().IsValid())
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand 1 has modifier FIXED, but has no value", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  if (!operands[1]->GetValue().IsValid())
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand 2 has modifier FIXED, but has no value", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  if (m_memoryCtx.GetElementType(operands[0]->GetValue()) == m_memoryCtx.GetElementType(operands[1]->GetValue()))
  {
    FinishExecutionSuccessfully();
  }
  else
  {
    FinishExecutionUnsuccessfully();
  }
  return SC_RESULT_OK;
}

}  // namespace scp
