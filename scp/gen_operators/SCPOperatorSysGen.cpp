/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpUtils.hpp"
#include "SCPOperatorSysGen.hpp"

namespace scp
{
using namespace utils;
SCPOperatorSysGen::SCPOperatorSysGen(ScMemoryContext &ctx, ScAddr addr)
  : SCPOperator(ctx, addr)
{
  operands = std::vector<SCPOperand*>(4);
}

std::string SCPOperatorSysGen::GetTypeName()
{
  return "sys_gen";
}

sc_result SCPOperatorSysGen::Parse()
{
  SCPOperator::Parse();
  ScIterator3Ptr const operandIt3 = m_memoryCtx.Iterator3(addr, ScType::EdgeAccessConstPosPerm, ScType::Unknown);
  while (operandIt3->Next())
  {
    auto * operand = new SCPOperand(m_memoryCtx, operandIt3->Get(1));

    if (!(operand->GetOrder() > 0 && operand->GetOrder() < 5 && operands[operand->GetOrder() - 1] == nullptr))
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

  for (size_t i = 0; i < 3; ++i)
  {
    auto * operand = operands[i];
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

sc_result SCPOperatorSysGen::Execute()
{
  if (operands[0]->IsAssign())
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand 1 must have FIXED modifier", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  if (operands[2]->IsAssign())
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand 3 must have FIXED modifier", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  if (operands[0]->IsFixed())
  {
    if (!operands[0]->GetValue().IsValid())
    {
#ifdef SCP_DEBUG
      Utils::logSCPError(m_memoryCtx, "Operand 1 has modifier FIXED, but has no value", addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }
  }

  if (operands[2]->IsFixed())
  {
    if (!operands[2]->GetValue().IsValid())
    {
#ifdef SCP_DEBUG
      Utils::logSCPError(m_memoryCtx, "Operand 3 has modifier FIXED, but has no value", addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }
  }

  ScTemplateParams generationParams;
  ScAddr const & generationParamsAddr = operands[2]->GetValue();
  ScIterator3Ptr const paramsMapIt3 = m_memoryCtx.Iterator3(
      generationParamsAddr, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);

  while (paramsMapIt3->Next())
  {
    ScAddr const & parameterPairAddr = paramsMapIt3->Get(2);

    ScIterator5Ptr const varIt5 = m_memoryCtx.Iterator5(
        parameterPairAddr, ScType::EdgeAccessConstPosPerm, ScType::Unknown, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_1);

    ScAddr varAddr;
    if (varIt5->Next())
      varAddr = SCPOperand(m_memoryCtx, varIt5->Get(1)).GetValue();
    else
    {
#ifdef SCP_DEBUG
      Utils::logSCPError(m_memoryCtx, "Generation parameters pair must have variable", addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    ScIterator5Ptr const replacementIt5 = m_memoryCtx.Iterator5(
        parameterPairAddr, ScType::EdgeAccessConstPosPerm, ScType::Unknown, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_2);

    ScAddr replacementAddr;
    if (replacementIt5->Next())
      replacementAddr = SCPOperand(m_memoryCtx, replacementIt5->Get(1)).GetValue();
    else
    {
#ifdef SCP_DEBUG
      Utils::logSCPError(m_memoryCtx, "Generation parameters pair must have replacement", addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    try
    {
      generationParams.Add(varAddr, replacementAddr);
    }
    catch (utils::ScException const & e)
    {
#ifdef SCP_DEBUG
      Utils::logSCPError(m_memoryCtx, "Generation parameters pair must has one replacement, but has two or more replacements", addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }
  }

  ScTemplate generationTemplate;
  ScAddr const & generationTemplateAddr = operands[0]->GetValue();

  try
  {
    m_memoryCtx.HelperBuildTemplate(generationTemplate, generationTemplateAddr, generationParams);
  }
  catch (utils::ScException const & e)
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, e.Message(), addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  ScTemplateGenResult generationResult;
  try
  {
    m_memoryCtx.HelperGenTemplate(generationTemplate, generationResult);
  }
  catch (utils::ScException const & e)
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, e.Message(), addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  std::set<ScAddr, ScAddrLessFunc> templateVarsSet;
  ScIterator3Ptr const templateIt3 = m_memoryCtx.Iterator3(
      generationTemplateAddr, ScType::EdgeAccessConstPosPerm, ScType::Unknown);
  while (templateIt3->Next())
  {
    ScAddr const & templateElementAddr = templateIt3->Get(2);
    if (!m_memoryCtx.GetElementType(templateElementAddr).IsVar())
      continue;

    templateVarsSet.insert(templateElementAddr);
  }

  ScAddr generatedElementsSetAddr;
  if (operands[3] != nullptr)
  {
    if (operands[3]->IsFixed())
    {
      generatedElementsSetAddr = operands[3]->GetValue();
    }
    else
    {
      generatedElementsSetAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
      operands[3]->ResetValue();
      operands[3]->SetValue(generatedElementsSetAddr);
    }
  }

  std::unordered_map<ScAddr, ScAddr, ScAddrHashFunc<size_t>> templateVarAddrsToGenerationResultAddrs;
  for (ScAddr const & templateVarAddr : templateVarsSet)
  {
    ScAddr generatedAddr;
    generationResult.Get(templateVarAddr, generatedAddr);
    if (!generatedAddr.IsValid())
      continue;

    ScAddr replacementsAddr;
    if (templateVarAddrsToGenerationResultAddrs.find(templateVarAddr) == templateVarAddrsToGenerationResultAddrs.cend())
    {
      replacementsAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
      templateVarAddrsToGenerationResultAddrs.insert({templateVarAddr, replacementsAddr});
    }
    else
    {
      replacementsAddr = templateVarAddrsToGenerationResultAddrs.at(templateVarAddr);
    }

    if (generatedElementsSetAddr.IsValid()
      && !m_memoryCtx.HelperCheckEdge(generatedElementsSetAddr, generatedAddr, ScType::EdgeAccessConstPosPerm))
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, generatedElementsSetAddr, generatedAddr);
  }

  ScAddr templateVarsToGenerationResultsSetAddr;
  if (operands[1]->IsFixed())
  {
    templateVarsToGenerationResultsSetAddr = operands[1]->GetValue();
  }
  else
  {
    templateVarsToGenerationResultsSetAddr = m_memoryCtx.CreateNode(ScType::NodeConst);;
    operands[1]->ResetValue();
    operands[1]->SetValue(templateVarsToGenerationResultsSetAddr);
  }

  for (auto const & pair : templateVarAddrsToGenerationResultAddrs)
  {
    ScAddr const & templateVarAddr = pair.first;
    ScAddr const & generationResultAddr = pair.second;

    ScAddr const & replacementPairAddr = m_memoryCtx.CreateEdge(
        ScType::EdgeDCommonConst, templateVarAddr, generationResultAddr);

    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, templateVarsToGenerationResultsSetAddr, replacementPairAddr);
  }

  FinishExecutionSuccessfully();
  return SC_RESULT_OK;
}

}  // namespace scp
