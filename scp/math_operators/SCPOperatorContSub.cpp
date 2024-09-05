/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "SCPOperatorContSub.hpp"
#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_stream.hpp"
#include <iostream>

namespace scp
{

SCPOperatorContSub::SCPOperatorContSub(ScMemoryContext & ctx, ScAddr addr)
  : SCPOperatorElStr3(ctx, addr)
{
}

std::string SCPOperatorContSub::GetTypeName()
{
  return "contSub";
}

sc_result SCPOperatorContSub::Parse()
{
  return SCPOperatorElStr3::Parse();
}

sc_result SCPOperatorContSub::Execute()
{
  std::cout << "SCPOperatorContSub execute(): start"
            << "\n";
  if (SC_RESULT_OK != ResetValues())
    return SC_RESULT_ERROR;

  if (!(operands[1]->IsFixed() && operands[2]->IsFixed()))
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Both operands must have FIXED modifier", addr);
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

  if (!operands[2]->GetValue().IsValid())
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand 2 has modifier FIXED, but has no value", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  std::string answer_str;

  if (Utils::scLinkContentIsInt(m_memoryCtx, operands[1]->GetValue())
      && Utils::scLinkContentIsInt(m_memoryCtx, operands[2]->GetValue()))
  {
    std::cout << "Links are int" << std::endl;
    int number1 = Utils::scLinkGetContentInt(m_memoryCtx, operands[1]->GetValue());
    int number2 = Utils::scLinkGetContentInt(m_memoryCtx, operands[2]->GetValue());
    int answer = int(number1 - number2);
    answer_str = std::to_string(answer);
  }

  if (Utils::scLinkContentIsUint(m_memoryCtx, operands[1]->GetValue())
      && Utils::scLinkContentIsUint(m_memoryCtx, operands[2]->GetValue()))
  {
    std::cout << "Links are uint" << std::endl;
    int number1 = Utils::scLinkGetContentUint(m_memoryCtx, operands[1]->GetValue());
    int number2 = Utils::scLinkGetContentUint(m_memoryCtx, operands[2]->GetValue());
    int answer = int(number1 - number2);
    answer_str = std::to_string(answer);
  }

  if (Utils::scLinkContentIsFloat(m_memoryCtx, operands[1]->GetValue())
      && Utils::scLinkContentIsFloat(m_memoryCtx, operands[2]->GetValue()))
  {
    std::cout << "Links are float" << std::endl;
    float number1 = Utils::scLinkGetContentFloat(m_memoryCtx, operands[1]->GetValue());
    float number2 = Utils::scLinkGetContentFloat(m_memoryCtx, operands[2]->GetValue());
    float answer = float(number1 - number2);
    answer_str = std::to_string(answer);
  }

  if (Utils::scLinkContentIsDouble(m_memoryCtx, operands[1]->GetValue())
      && Utils::scLinkContentIsDouble(m_memoryCtx, operands[2]->GetValue()))
  {
    std::cout << "Links are double" << std::endl;
    double number1 = Utils::scLinkGetContentDouble(m_memoryCtx, operands[1]->GetValue());
    double number2 = Utils::scLinkGetContentDouble(m_memoryCtx, operands[2]->GetValue());
    double answer = double(number1 - number2);
    answer_str = std::to_string(answer);
  }

  if (answer_str.empty())
  {
    if (!Utils::scLinkPlainNumbers(m_memoryCtx, operands[1]->GetValue()).empty()
        && !Utils::scLinkPlainNumbers(m_memoryCtx, operands[2]->GetValue()).empty())
    {
      std::string str1 = Utils::scLinkPlainNumbers(m_memoryCtx, operands[1]->GetValue());
      std::string str2 = Utils::scLinkPlainNumbers(m_memoryCtx, operands[2]->GetValue());
      std::string intInit = "int: ";
      std::string::size_type i1 = str1.find(intInit);
      std::string::size_type i2 = str2.find(intInit);

      if (i1 != std::string::npos && i2 != std::string::npos)
      {
        std::cout << "Link is INT" << std::endl;
        str1.erase(i1, intInit.length());
        str2.erase(i2, intInit.length());
        int num1 = std::atoi(str1.c_str());
        int num2 = std::atoi(str2.c_str());
        int answer = num1 - num2;
        answer_str = std::to_string(answer);
      }

      std::string intDouble = "double: ";
      std::string::size_type d1 = str1.find(intDouble);
      std::string::size_type d2 = str2.find(intDouble);

      if ((d1 != std::string::npos) && (d2 != std::string::npos))
      {
        std::cout << "Link is Double" << std::endl;
        str1.erase(d1, intDouble.length());
        str2.erase(d2, intDouble.length());
        double num1 = std::stod(str1);
        double num2 = std::stod(str2);
        double answer = num1 - num2;
        answer_str = std::to_string(answer);
      }
    }
    else
    {
      FinishExecutionUnsuccessfully();
#ifdef SCP_DEBUG
      Utils::logSCPError(m_memoryCtx, "Both operands are not numeric!", addr);
#endif
      return SC_RESULT_OK;
    }
  }

  ScAddr answerLink = m_memoryCtx.CreateLink();
  ScStreamPtr streamPtr = Utils::StreamFromString(answer_str);
  m_memoryCtx.SetLinkContent(answerLink, streamPtr);

  operands[0]->ResetValue();
  operands[0]->SetValue(answerLink);

  return SC_RESULT_OK;
}

}  // namespace scp
