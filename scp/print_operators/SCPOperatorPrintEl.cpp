/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "SCPOperatorPrintEl.hpp"
#include "sc-memory/sc_memory.hpp"
#include <iostream>

namespace scp
{

SCPOperatorPrintEl::SCPOperatorPrintEl(ScAgentContext & ctx, ScAddr addr)
  : SCPOperatorElStr1(ctx, addr)
{
}

std::string SCPOperatorPrintEl::GetTypeName()
{
  return "printEl";
}

sc_result SCPOperatorPrintEl::Parse()
{
  return SCPOperatorElStr1::Parse();
}

sc_result SCPOperatorPrintEl::Execute()
{
  if (SC_RESULT_OK != ResetValues())
    return SC_RESULT_ERROR;

  if (!operands[0]->IsFixed())
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand must have FIXED modifier", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }
  if (!operands[0]->GetValue().IsValid())
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand has modifier FIXED, but has no value", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  Utils::printInfo(m_memoryCtx, operands[0]->GetValue());

  FinishExecutionSuccessfully();
  return SC_RESULT_OK;
}

}  // namespace scp
