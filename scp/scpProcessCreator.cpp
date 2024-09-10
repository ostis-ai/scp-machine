/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpProcessCreator.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

namespace scp
{
ScResult ASCPProcessCreator::DoProgram(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event,
    ScAction & action)
{
  auto const & [program, params] = action.GetArguments<2>();
  if (!program.IsValid() || !params.IsValid())
    return action.FinishUnsuccessfully();

  ScTemplate program_templ;
  m_context.HelperBuildTemplate(program_templ, program);
  ScTemplateParams gen_params;

  ScAddr process_node;
  auto const & programKeyElementIterator = m_context.Iterator5(
      program,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeVar,
      ScType::EdgeAccessConstPosPerm,
      Keynodes::rrel_key_sc_element);
  if (programKeyElementIterator->IsValid() && programKeyElementIterator->Next())
    process_node = programKeyElementIterator->Get(2);
  else
    return action.FinishUnsuccessfully();

  auto const & processParametersIterator = m_context.Iterator5(
      process_node, ScType::EdgeAccessVarPosPerm, ScType::Unknown, ScType::EdgeAccessConstPosPerm, program);
  if (!processParametersIterator->IsValid())
  {
    return action.FinishUnsuccessfully();
  }

  //! TODO Make all sc-links constant to allow using constant sc-links within scp-program code
  while (processParametersIterator->Next())
  {
    ScAddr order;
    if (Utils::resolveOrderRoleRelation(m_context, processParametersIterator->Get(1), order))
    {
      auto const & iter_param = m_context.Iterator5(
          params, ScType::EdgeAccessConstPosPerm, ScType::Unknown, ScType::EdgeAccessConstPosPerm, order);
      if (!iter_param->Next())
      {
#ifdef SCP_DEBUG
        Utils::logSCPError(m_context, "Missed scp-program parameter", order);
#endif
        continue;
      }
      gen_params.Add(processParametersIterator->Get(2), iter_param->Get(2));
    }
  }

  ScTemplateGenResult result;
  m_context.HelperGenTemplate(program_templ, result, gen_params);

  ScAddr const_process_node = result[process_node];
  auto const & generatedProcessDecompositionIterator = m_context.Iterator5(
      ScType::NodeConst,
      ScType::EdgeDCommonConst,
      const_process_node,
      ScType::EdgeAccessConstPosPerm,
      Keynodes::nrel_decomposition_of_action);
  if (generatedProcessDecompositionIterator->Next())
  {
    ScIterator5Ptr oper_iter = m_context.Iterator5(
        generatedProcessDecompositionIterator->Get(0),
        ScType::EdgeAccessConstPosPerm,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        Keynodes::rrel_1);
    if (oper_iter->Next())
    {
      action.SetResult(const_process_node);
      m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::active_action, oper_iter->Get(2));
    }
    else
    {
#ifdef SCP_DEBUG
      Utils::logSCPError(m_context, "Missed initial scp-operator", program);
#endif
      m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::action_finished, const_process_node);
    }
  }
  else
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_context, "Missed scp-process decomposition", program);
#endif
    m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::action_finished, const_process_node);
  }

  return action.FinishSuccessfully();
}

ScAddr ASCPProcessCreator::GetEventSubscriptionElement() const noexcept(false)
{
  return Keynodes::action_initiated;
}

ScAddr ASCPProcessCreator::GetActionClass() const
{
  return Keynodes::action_create_process;
}

bool ASCPProcessCreator::CheckInitiationCondition(ScActionInitiatedEvent const & event)
{
  return m_context.HelperCheckEdge(
      Keynodes::action_scp_interpretation_request, event.GetOtherElement(), ScType::EdgeAccessConstPosPerm);
}
}  // namespace scp
