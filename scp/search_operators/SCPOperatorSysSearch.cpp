/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpUtils.hpp"
#include "SCPOperatorSysSearch.hpp"

namespace scp
{
using namespace utils;

SCPOperatorSysSearch::SCPOperatorSysSearch(ScAgentContext & ctx, ScAddr addr)
  : SCPOperator(ctx, addr)
{
  operands = std::vector<SCPOperand *>(4);
}

std::string SCPOperatorSysSearch::GetTypeName()
{
  return "sys_search";
}

sc_result SCPOperatorSysSearch::Parse()
{
  SCPOperator::Parse();
  ScIterator3Ptr const operandIt3 = m_memoryCtx.CreateIterator3(addr, ScType::ConstPermPosArc, ScType::Unknown);
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

sc_result SCPOperatorSysSearch::Execute()
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

  ScTemplateParams searchParams;
  ScAddr const & searchParamsAddr = operands[2]->GetValue();
  ScIterator3Ptr const paramsMapIt3 =
      m_memoryCtx.CreateIterator3(searchParamsAddr, ScType::ConstPermPosArc, ScType::ConstNode);

  while (paramsMapIt3->Next())
  {
    ScAddr const & parameterPairAddr = paramsMapIt3->Get(2);

    ScIterator5Ptr const varIt5 = m_memoryCtx.CreateIterator5(
        parameterPairAddr, ScType::ConstPermPosArc, ScType::Unknown, ScType::ConstPermPosArc, Keynodes::rrel_1);

    ScAddr varAddr;
    if (varIt5->Next())
      varAddr = SCPOperand(m_memoryCtx, varIt5->Get(1)).GetValue();
    else
    {
#ifdef SCP_DEBUG
      Utils::logSCPError(m_memoryCtx, "Search parameters pair must have variable", addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    ScIterator5Ptr const replacementIt5 = m_memoryCtx.CreateIterator5(
        parameterPairAddr, ScType::ConstPermPosArc, ScType::Unknown, ScType::ConstPermPosArc, Keynodes::rrel_2);

    ScAddr replacementAddr;
    if (replacementIt5->Next())
      replacementAddr = SCPOperand(m_memoryCtx, replacementIt5->Get(1)).GetValue();
    else
    {
#ifdef SCP_DEBUG
      Utils::logSCPError(m_memoryCtx, "Search parameters pair must have replacement", addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    try
    {
      searchParams.Add(varAddr, replacementAddr);
    }
    catch (utils::ScException const & e)
    {
#ifdef SCP_DEBUG
      Utils::logSCPError(
          m_memoryCtx, "Search parameters pair must has one replacement, but has two or more replacements", addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }
  }

  ScTemplate searchTemplate;
  ScAddr const & searchTemplateAddr = operands[0]->GetValue();

  try
  {
    m_memoryCtx.BuildTemplate(searchTemplate, searchTemplateAddr, searchParams);
  }
  catch (utils::ScException const & e)
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, e.Message(), addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  ScTemplateSearchResult searchResult;
  try
  {
    m_memoryCtx.SearchByTemplate(searchTemplate, searchResult);
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
  ScIterator3Ptr const templateIt3 =
      m_memoryCtx.CreateIterator3(searchTemplateAddr, ScType::ConstPermPosArc, ScType::Unknown);
  while (templateIt3->Next())
  {
    ScAddr const & templateElementAddr = templateIt3->Get(2);
    if (!m_memoryCtx.GetElementType(templateElementAddr).IsVar())
      continue;

    templateVarsSet.insert(templateElementAddr);
  }

  ScAddr foundElementsSetAddr;
  if (operands[3] != nullptr)
  {
    if (operands[3]->IsFixed())
    {
      foundElementsSetAddr = operands[3]->GetValue();
    }
    else
    {
      foundElementsSetAddr = m_memoryCtx.GenerateNode(ScType::ConstNode);
      operands[3]->ResetValue();
      operands[3]->SetValue(foundElementsSetAddr);
    }
  }

  std::unordered_map<ScAddr, ScAddr, ScAddrHashFunc> templateVarAddrsToSearchResultAddrs;
  searchResult.ForEach(
      [this, &templateVarsSet, &templateVarAddrsToSearchResultAddrs, &foundElementsSetAddr](
          ScTemplateResultItem const & item)
      {
        for (ScAddr const & templateVarAddr : templateVarsSet)
        {
          ScAddr foundAddr;
          item.Get(templateVarAddr, foundAddr);
          if (!foundAddr.IsValid())
            continue;

          ScAddr replacementsSetAddr;
          if (templateVarAddrsToSearchResultAddrs.find(templateVarAddr) == templateVarAddrsToSearchResultAddrs.cend())
          {
            replacementsSetAddr = m_memoryCtx.GenerateNode(ScType::ConstNode);
            templateVarAddrsToSearchResultAddrs.insert({templateVarAddr, replacementsSetAddr});
          }
          else
          {
            replacementsSetAddr = templateVarAddrsToSearchResultAddrs.at(templateVarAddr);
          }

          if (!m_memoryCtx.CheckConnector(replacementsSetAddr, foundAddr, ScType::ConstPermPosArc))
            m_memoryCtx.GenerateConnector(ScType::ConstPermPosArc, replacementsSetAddr, foundAddr);

          if (foundElementsSetAddr.IsValid()
              && !m_memoryCtx.CheckConnector(foundElementsSetAddr, foundAddr, ScType::ConstPermPosArc))
            m_memoryCtx.GenerateConnector(ScType::ConstPermPosArc, foundElementsSetAddr, foundAddr);
        }
      });

  ScAddr templateVarsToSearchResultsSetAddr;
  if (operands[1]->IsFixed())
  {
    templateVarsToSearchResultsSetAddr = operands[1]->GetValue();
  }
  else
  {
    templateVarsToSearchResultsSetAddr = m_memoryCtx.GenerateNode(ScType::ConstNode);
    ;
    operands[1]->ResetValue();
    operands[1]->SetValue(templateVarsToSearchResultsSetAddr);
  }

  for (auto const & pair : templateVarAddrsToSearchResultAddrs)
  {
    ScAddr const & templateVarAddr = pair.first;
    ScAddr const & searchResultAddr = pair.second;

    ScAddr const & searchResultSetVariableAddr = m_memoryCtx.GenerateNode(ScType::VarNode);
    m_memoryCtx.GenerateConnector(ScType::ConstTempPosArc, searchResultSetVariableAddr, searchResultAddr);

    ScAddr const & replacementPairAddr = m_memoryCtx.GenerateNode(ScType::ConstNode);

    ScAddr const & edgeToTemplateVarAddr =
        m_memoryCtx.GenerateConnector(ScType::ConstPermPosArc, replacementPairAddr, templateVarAddr);
    m_memoryCtx.GenerateConnector(ScType::ConstPermPosArc, Keynodes::rrel_1, edgeToTemplateVarAddr);
    m_memoryCtx.GenerateConnector(ScType::ConstPermPosArc, Keynodes::rrel_scp_const, edgeToTemplateVarAddr);

    ScAddr const & edgeToSearchResultSetVariableAddr =
        m_memoryCtx.GenerateConnector(ScType::ConstPermPosArc, replacementPairAddr, searchResultSetVariableAddr);
    m_memoryCtx.GenerateConnector(ScType::ConstPermPosArc, Keynodes::rrel_2, edgeToSearchResultSetVariableAddr);
    m_memoryCtx.GenerateConnector(ScType::ConstPermPosArc, Keynodes::rrel_scp_var, edgeToSearchResultSetVariableAddr);
    m_memoryCtx.GenerateConnector(ScType::ConstPermPosArc, Keynodes::rrel_assign, edgeToSearchResultSetVariableAddr);
    m_memoryCtx.GenerateConnector(ScType::ConstPermPosArc, Keynodes::rrel_set, edgeToSearchResultSetVariableAddr);

    m_memoryCtx.GenerateConnector(ScType::ConstPermPosArc, templateVarsToSearchResultsSetAddr, replacementPairAddr);
  }

  if (searchResult.IsEmpty())
  {
    FinishExecutionUnsuccessfully();
    return SC_RESULT_ERROR_NOT_FOUND;
  }
  else
  {
    FinishExecutionSuccessfully();
    return SC_RESULT_OK;
  }
}

}  // namespace scp
