/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpUtils.hpp"
#include "SCPOperatorContRandom.hpp"

#include <random>

namespace scp
{

SCPOperatorContRandom::SCPOperatorContRandom(ScAgentContext & ctx, ScAddr addr)
  : SCPOperatorElStr3(ctx, addr)
{
}

std::string SCPOperatorContRandom::GetTypeName()
{
  return "contRandom";
}

sc_result SCPOperatorContRandom::Parse()
{
  return SCPOperatorElStr3::Parse();
}

sc_result SCPOperatorContRandom::Execute()
{
  if (SC_RESULT_OK != ResetValues())
    return SC_RESULT_ERROR;
  ScAddr lowerBound;
  ScAddr upperBound;

  if (operands[1]->IsFixed())
  {
    lowerBound = operands[1]->GetValue();
    if (!lowerBound.IsValid())
    {
#ifdef SCP_DEBUG
      Utils::logSCPError(m_memoryCtx, "Operand 2 has FIXED modifier, but has no value", addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }
  }
  else
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand 2 must have FIXED modifier", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  if (operands[2]->IsFixed())
  {
    upperBound = operands[2]->GetValue();
    if (!upperBound.IsValid())
    {
#ifdef SCP_DEBUG
      Utils::logSCPError(m_memoryCtx, "Operand 3 has FIXED modifier, but has no value", addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }
  }
  else
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand 3 must have FIXED modifier", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  if (!m_memoryCtx.GetElementType(lowerBound).IsLink())
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(
        m_memoryCtx,
        "Operand 2 value has to be a link, got " + std::string(m_memoryCtx.GetElementType(lowerBound)) + " instead",
        addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  if (!m_memoryCtx.GetElementType(upperBound).IsLink())
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(
        m_memoryCtx,
        "Operand 3 value has to be a link, got " + std::string(m_memoryCtx.GetElementType(upperBound)) + " instead",
        addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }
  try
  {
    std::string answer_str;

    if (Utils::scLinkContentIsInt(m_memoryCtx, lowerBound) && Utils::scLinkContentIsInt(m_memoryCtx, upperBound))
    {
      std::cout << "Links are int" << std::endl;
      int number1 = Utils::scLinkGetContentInt(m_memoryCtx, lowerBound);
      int number2 = Utils::scLinkGetContentInt(m_memoryCtx, upperBound);
      auto randomNumber = randomInt(number1, number2);
      answer_str = std::to_string(randomNumber);
    }
    if (Utils::scLinkContentIsUint(m_memoryCtx, lowerBound) && Utils::scLinkContentIsUint(m_memoryCtx, upperBound))
    {
      std::cout << "Links are uint" << std::endl;
      int number1 = Utils::scLinkGetContentUint(m_memoryCtx, lowerBound);
      int number2 = Utils::scLinkGetContentUint(m_memoryCtx, upperBound);
      auto randomNumber = randomInt(number1, number2);
      answer_str = std::to_string(randomNumber);
    }
    if (Utils::scLinkContentIsFloat(m_memoryCtx, lowerBound) && Utils::scLinkContentIsFloat(m_memoryCtx, upperBound))
    {
      std::cout << "Links are float" << std::endl;
      float number1 = Utils::scLinkGetContentFloat(m_memoryCtx, lowerBound);
      float number2 = Utils::scLinkGetContentFloat(m_memoryCtx, upperBound);
      auto randomNumber = randomReal(number1, number2);
      answer_str = std::to_string(randomNumber);
    }
    if (Utils::scLinkContentIsDouble(m_memoryCtx, lowerBound) && Utils::scLinkContentIsDouble(m_memoryCtx, upperBound))
    {
      std::cout << "Links are double" << std::endl;
      double number1 = Utils::scLinkGetContentDouble(m_memoryCtx, lowerBound);
      double number2 = Utils::scLinkGetContentDouble(m_memoryCtx, upperBound);
      auto randomNumber = randomReal(number1, number2);
      answer_str = std::to_string(randomNumber);
    }

    if (answer_str.empty())
    {
      std::string str1 = Utils::scLinkPlainNumbers(m_memoryCtx, operands[1]->GetValue());
      std::string str2 = Utils::scLinkPlainNumbers(m_memoryCtx, operands[2]->GetValue());
      if (!str1.empty() && !str2.empty())
      {
        std::string intInit = "int: ";
        std::string::size_type i1 = str1.find(intInit);
        std::string::size_type i2 = str2.find(intInit);

        if (i1 != std::string::npos && i2 != std::string::npos)
        {
          std::cout << "Link is INT" << std::endl;
          str1.erase(i1, intInit.length());
          str2.erase(i2, intInit.length());
          int number1 = std::atoi(str1.c_str());
          int number2 = std::atoi(str2.c_str());
          auto randomNumber = randomInt(number1, number2);
          answer_str = std::to_string(randomNumber);
        }

        std::string intDouble = "double: ";
        std::string::size_type d1 = str1.find(intDouble);
        std::string::size_type d2 = str2.find(intDouble);

        if ((d1 != std::string::npos) && (d2 != std::string::npos))
        {
          std::cout << "Link is Double" << std::endl;
          str1.erase(d1, intDouble.length());
          str2.erase(d2, intDouble.length());
          double number1 = std::stod(str1);
          double number2 = std::stod(str2);
          auto randomNumber = randomReal(number1, number2);
          answer_str = std::to_string(randomNumber);
        }
      }
      else
      {
        FinishExecutionUnsuccessfully();
#ifdef SCP_DEBUG
        Utils::logSCPError(m_memoryCtx, "Operands 2 and 3 are not numeric!", addr);
#endif
        return SC_RESULT_OK;
      }
    }

    ScAddr answerLink = m_memoryCtx.GenerateLink();
    ScStreamPtr streamPtr = Utils::StreamFromString(answer_str);
    m_memoryCtx.SetLinkContent(answerLink, streamPtr);

    operands[0]->ResetValue();
    operands[0]->SetValue(answerLink);

    FinishExecutionSuccessfully();
    return SC_RESULT_OK;
  }
  catch (utils::ScException const & exception)
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, exception.Description(), addr);
#endif
    FinishExecutionUnsuccessfully();
    return SC_RESULT_ERROR;
  }
}

template <typename T>
T SCPOperatorContRandom::randomReal(T lowerBound, T upperBound)
{
  if (lowerBound > upperBound)
    SC_THROW_EXCEPTION(utils::ExceptionInvalidParams, "Operand 2 shouldn't be greater then operand 3");
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_real_distribution<T> distribution(lowerBound, upperBound);
  return distribution(generator);
}

template <typename T>
T SCPOperatorContRandom::randomInt(T lowerBound, T upperBound)
{
  if (lowerBound > upperBound)
    SC_THROW_EXCEPTION(utils::ExceptionInvalidParams, "Operand 2 shouldn't be greater then operand 3");
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<T> distribution(lowerBound, upperBound);
  return distribution(generator);
}
}  // namespace scp
