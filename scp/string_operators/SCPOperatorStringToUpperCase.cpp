/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpUtils.hpp"
#include "SCPOperatorStringToUpperCase.hpp"
#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_stream.hpp"
#include <iostream>

using namespace std;

namespace scp
{

SCPOperatorStringToUpperCase::SCPOperatorStringToUpperCase(ScAgentContext & ctx, ScAddr addr)
  : SCPOperatorElStr2(ctx, addr)
{
}

string SCPOperatorStringToUpperCase::GetTypeName()
{
  return "stringToUpperCase";
}

sc_result SCPOperatorStringToUpperCase::Parse()
{
  return SCPOperatorElStr2::Parse();
}

sc_result SCPOperatorStringToUpperCase::Execute()
{
  if (SC_RESULT_OK != ResetValues())
    return SC_RESULT_ERROR;

  if (!operands[1]->IsFixed())
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand must have FIXED modifier", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }
  if (!operands[1]->GetValue().IsValid())
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand 1 has modifier FIXED, but has no value", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  string str1 = Utils::scLinkGetString(m_memoryCtx, operands[1]->GetValue());
  if (str1 == "")
  {
    cout << "Link content is empty!" << endl;
    FinishExecutionUnsuccessfully();
    return SC_RESULT_OK;
  }

  utils::StringUtils::ToUpperCase(str1);
  ScStreamPtr streamPtr = Utils::StreamFromString(str1);
  ScAddr answerLink = m_memoryCtx.GenerateLink();

  m_memoryCtx.SetLinkContent(answerLink, streamPtr);

  Utils::printOperatorAnswer(m_memoryCtx, operands[0], answerLink);

  FinishExecutionSuccessfully();

  return SC_RESULT_OK;
}

}  // namespace scp
