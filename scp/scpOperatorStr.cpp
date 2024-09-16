/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-memory/sc_addr.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "scpOperatorStr.hpp"
#include <iostream>
#include <sstream>

namespace scp
{

SCPOperatorElStr1::SCPOperatorElStr1(ScAgentContext & ctx_, ScAddr addr_)
  : SCPOperator(ctx_, addr_)
{
  operands = std::vector<SCPOperand *>(1);
}

sc_result SCPOperatorElStr1::Parse()
{
  SCPOperator::Parse();
  ScIterator3Ptr iter_operator = m_memoryCtx.CreateIterator3(addr, ScType::EdgeAccessConstPosPerm, ScType(0));
  while (iter_operator->Next())
  {
    SCPOperand * operand = new SCPOperand(m_memoryCtx, iter_operator->Get(1));
    if (!(operand->GetOrder() == 1 && operands[0] == nullptr))
    {
#ifdef SCP_DEBUG
      std::stringstream ss;
      ss << "Invalid operand order ";
      ss << (int)operand->GetOrder();
      Utils::logSCPError(m_memoryCtx, ss.str(), addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }
    else
    {
      operands[operand->GetOrder() - 1] = operand;
    }
  }
  return CheckNullValues();
}

SCPOperatorElStr2::SCPOperatorElStr2(ScAgentContext & ctx_, ScAddr addr_)
  : SCPOperator(ctx_, addr_)
{
  operands = std::vector<SCPOperand *>(2);
}

sc_result SCPOperatorElStr2::Parse()
{
  SCPOperator::Parse();
  ScIterator3Ptr iter_operator = m_memoryCtx.CreateIterator3(addr, ScType::EdgeAccessConstPosPerm, ScType(0));
  while (iter_operator->Next())
  {
    SCPOperand * operand = new SCPOperand(m_memoryCtx, iter_operator->Get(1));
    if (!(operand->GetOrder() > 0 && operand->GetOrder() < 3 && operands[operand->GetOrder() - 1] == nullptr))
    {
#ifdef SCP_DEBUG
      std::stringstream ss;
      ss << "Invalid operand order ";
      ss << (int)operand->GetOrder();
      Utils::logSCPError(m_memoryCtx, ss.str(), addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }
    else
    {
      operands[operand->GetOrder() - 1] = operand;
    }
  }
  return CheckNullValues();
}

SCPOperatorElStr3::SCPOperatorElStr3(ScAgentContext & ctx_, ScAddr addr_)
  : SCPOperator(ctx_, addr_)
{
  operands = std::vector<SCPOperand *>(3);
}

sc_result SCPOperatorElStr3::Parse()
{
  SCPOperator::Parse();
  ScIterator3Ptr iter_operator = m_memoryCtx.CreateIterator3(addr, ScType::EdgeAccessConstPosPerm, ScType(0));
  while (iter_operator->Next())
  {
    SCPOperand * operand = new SCPOperand(m_memoryCtx, iter_operator->Get(1));
    if (!(operand->GetOrder() > 0 && operand->GetOrder() < 4 && operands[operand->GetOrder() - 1] == nullptr))
    {
#ifdef SCP_DEBUG
      std::stringstream ss;
      ss << "Invalid operand order ";
      ss << (int)operand->GetOrder();
      Utils::logSCPError(m_memoryCtx, ss.str(), addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }
    else
    {
      operands[operand->GetOrder() - 1] = operand;
    }
  }
  return CheckNullValues();
}

SCPOperatorElStr5::SCPOperatorElStr5(ScAgentContext & ctx_, ScAddr addr_)
  : SCPOperator(ctx_, addr_)
{
  operands = std::vector<SCPOperand *>(5);
}

sc_result SCPOperatorElStr5::Parse()
{
  SCPOperator::Parse();
  ScIterator3Ptr iter_operator = m_memoryCtx.CreateIterator3(addr, ScType::EdgeAccessConstPosPerm, ScType(0));
  while (iter_operator->Next())
  {
    SCPOperand * operand = new SCPOperand(m_memoryCtx, iter_operator->Get(1));
    if (!(operand->GetOrder() > 0 && operand->GetOrder() < 6 && operands[operand->GetOrder() - 1] == nullptr))
    {
#ifdef SCP_DEBUG
      std::stringstream ss;
      ss << "Invalid operand order ";
      ss << (int)operand->GetOrder();
      Utils::logSCPError(m_memoryCtx, ss.str(), addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }
    else
    {
      operands[operand->GetOrder() - 1] = operand;
    }
  }
  return CheckNullValues();
}

}  // namespace scp
