/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-memory/sc_addr.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "SCPOperatorCall.hpp"
#include <iostream>
#include <sstream>

namespace scp
{

SCPOperatorCall::SCPOperatorCall(ScMemoryContext & ctx_, ScAddr addr_)
  : SCPOperatorElStr3(ctx_, addr_)
{
  //! TODO Change 10 to some correct value
  params = std::vector<SCPOperand *>(10);
  std::fill(params.begin(), params.end(), nullptr);

  expected_params = std::vector<SCPParameter *>(10);
  std::fill(expected_params.begin(), expected_params.end(), nullptr);
}

std::string SCPOperatorCall::GetTypeName()
{
  return "call";
}

sc_result SCPOperatorCall::Parse()
{
  if (SC_RESULT_OK != SCPOperatorElStr3::Parse())
  {
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  ScAddr param_set;
  if (operands[1]->IsFixed())
  {
    if (!operands[1]->GetValue().IsValid())
    {
#ifdef SCP_DEBUG
      Utils::logSCPError(m_memoryCtx, "Operand 2 has modifier FIXED, but has no value", addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }
    param_set = operands[1]->GetValue();
  }
  else
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand 2 must have FIXED modifier", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  ScIterator3Ptr iter_param = m_memoryCtx.Iterator3(param_set, ScType::EdgeAccessConstPosPerm, ScType(0));
  while (iter_param->Next())
  {
    auto * operand = new SCPOperand(m_memoryCtx, iter_param->Get(1));
    if (!(operand->GetOrder() > 0 && params[operand->GetOrder() - 1] == nullptr))
    {
#ifdef SCP_DEBUG
      std::stringstream ss;
      ss << "Invalid param order ";
      ss << (int)operand->GetOrder();
      Utils::logSCPError(m_memoryCtx, ss.str(), addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }
    else
    {
      params[operand->GetOrder() - 1] = operand;
    }
  }
  return SC_RESULT_OK;
}

sc_result SCPOperatorCall::Execute()
{
  if (SC_RESULT_OK != ResetValues())
    return SC_RESULT_ERROR;

  ScAddr program_node;
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
    program_node = operands[0]->GetValue();
  }
  else
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand 1 must have FIXED modifier", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  if (operands[2]->IsFixed())
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_memoryCtx, "Operand 3 must have ASSIGN modifier", addr);
#endif
    FinishExecutionWithError();
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  ScAddr process_node;
  ScIterator5Ptr iter_temp = m_memoryCtx.Iterator5(
      program_node,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeVar,
      ScType::EdgeAccessConstPosPerm,
      Keynodes::rrel_key_sc_element);
  if (iter_temp->IsValid() && iter_temp->Next())
    process_node = iter_temp->Get(2);
  else
    return SC_RESULT_ERROR_INVALID_PARAMS;

  ScIterator5Ptr iter_params = m_memoryCtx.Iterator5(
      process_node, ScType::EdgeAccessVarPosPerm, ScType::NodeVar, ScType::EdgeAccessConstPosPerm, program_node);
  while (iter_params->Next())
  {
    auto * param = new SCPParameter(m_memoryCtx, iter_params->Get(1));
    if (!(param->GetOrder() > 0 && expected_params[param->GetOrder() - 1] == nullptr))
    {
#ifdef SCP_DEBUG
      std::stringstream ss;
      ss << "Invalid program param order ";
      ss << (int)param->GetOrder();
      Utils::logSCPError(m_memoryCtx, ss.str(), addr);
#endif
      FinishExecutionWithError();
      return SC_RESULT_ERROR_INVALID_PARAMS;
    }
    else
    {
      expected_params[param->GetOrder() - 1] = param;
    }
  }

  ScAddr params_set = m_memoryCtx.CreateNode(ScType::NodeConst);

  for (size_t i = 0; i < expected_params.size(); i++)
  {
    if (expected_params[i] != nullptr)
    {
      if (params[i] == nullptr)
      {
#ifdef SCP_DEBUG
        std::stringstream ss;
        ss << "Expected parameter " << (i + 1) << " was not passed";
        Utils::logSCPError(m_memoryCtx, ss.str(), addr);
#endif
        m_memoryCtx.EraseElement(params_set);
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
      }
      ScAddr role_rel, arc;
      Utils::resolveOrderRoleRelation(m_memoryCtx, i + 1, role_rel);

      if (expected_params[i]->IsIn())
      {
        if (params[i]->IsFixed())
        {
          if (!params[i]->GetValue().IsValid())
          {
#ifdef SCP_DEBUG
            std::stringstream ss;
            ss << "Passed param " << (i + 1) << " has modifier FIXED, but has no value";
            Utils::logSCPError(m_memoryCtx, ss.str(), addr);
#endif
            m_memoryCtx.EraseElement(params_set);
            FinishExecutionWithError();
            return SC_RESULT_ERROR_INVALID_PARAMS;
          }
          program_node = operands[0]->GetValue();
        }
        else
        {
#ifdef SCP_DEBUG
          std::stringstream ss;
          ss << "Passed param " << (i + 1) << " must have FIXED modifier, because corresponds to IN parameter";
          Utils::logSCPError(m_memoryCtx, ss.str(), addr);
#endif
          m_memoryCtx.EraseElement(params_set);
          FinishExecutionWithError();
          return SC_RESULT_ERROR_INVALID_PARAMS;
        }
        arc = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, params_set, params[i]->GetValue());
      }
      else
      {
        arc = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, params_set, params[i]->GetAddr());
      }
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, role_rel, arc);
    }
  }

  ScAddr scp_quest = m_memoryCtx.CreateNode(ScType::NodeConst);
  ScAddr arc1 = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, scp_quest, program_node);
  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::rrel_1, arc1);

  arc1 = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, scp_quest, params_set);
  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::rrel_2, arc1);

  arc1 = m_memoryCtx.CreateEdge(ScType::EdgeDCommonConst, scp_quest, Keynodes::abstract_scp_machine);
  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::nrel_authors, arc1);

  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::action_scp_interpretation_request, scp_quest);
  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::action_initiated, scp_quest);

  operands[2]->SetValue(scp_quest);

#ifdef SCP_DEBUG
  std::cout << "====Called scp-program: ";
  Utils::printSystemIdentifier(m_memoryCtx, program_node);
  std::cout << std::endl;
#endif

  FinishExecutionSuccessfully();

  return SC_RESULT_OK;
}

SCPOperatorCall::~SCPOperatorCall()
{
  for (auto & param : params)
  {
    delete param;
  }
  for (auto & expected_param : expected_params)
  {
    delete expected_param;
  }
}

}  // namespace scp
