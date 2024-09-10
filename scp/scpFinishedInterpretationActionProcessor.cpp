/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpFinishedInterpretationActionProcessor.hpp"

#include "scpKeynodes.hpp"
#include "scpOperator.hpp"

namespace scp
{

ScResult ASCPFinishedInterpretationActionProcessor::DoProgram(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event,
    ScAction & action)
{
  ScAction scp_action = m_context.ConvertToAction(event.GetOtherElement());
  ScAddr const & scpActionParams = scp_action.GetArgument(2);
  if (m_context.IsElement(scpActionParams))
  {
    auto const & scpParamConnectorIterator = m_context.Iterator5(
        scpActionParams,
        ScType::EdgeAccessConstPosPerm,
        ScType::EdgeAccessConstPosPerm,
        ScType::EdgeAccessConstPosPerm,
        Keynodes::rrel_2);
    if (scpParamConnectorIterator->Next())
    {
      ScAddr const & scpParamConnector = scpParamConnectorIterator->Get(2);
      ScAddr connectorSource, connectorTarget;
      m_context.GetEdgeInfo(scpParamConnector, connectorSource, connectorTarget);
      if (connectorSource == Keynodes::action_initiated
          && !m_context.HelperCheckEdge(Keynodes::action_finished, connectorTarget, ScType::EdgeAccessConstPosPerm))
        m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::action_finished, connectorTarget);
    }
  }

  ScAddr wait_operator;

  // SCP_CONST case
  ScIterator5Ptr iter = m_context.Iterator5(
      ScType::NodeConst,
      ScType::EdgeAccessConstPosPerm,
      scp_action,
      ScType::EdgeAccessConstPosPerm,
      Keynodes::rrel_scp_const);
  while (iter->Next())
  {
    wait_operator = iter->Get(0);
    if (!m_context.HelperCheckEdge(Keynodes::op_waitReturn, wait_operator, ScType::EdgeAccessConstPosPerm)
        || !m_context.HelperCheckEdge(Keynodes::active_action, wait_operator, ScType::EdgeAccessConstPosPerm))
      continue;

    SCPOperator::FinishExecutionSuccessfully(m_context, wait_operator);
    break;
  }

  // SCP_VAR case
  ScIterator3Ptr iter3 = m_context.Iterator3(ScType::NodeConst, ScType::EdgeAccessConstPosTemp, scp_action);
  while (iter3->Next())
  {
    ScIterator5Ptr iter5 = m_context.Iterator5(
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        iter3->Get(0),
        ScType::EdgeAccessConstPosPerm,
        Keynodes::rrel_scp_var);
    while (iter5->Next())
    {
      wait_operator = iter5->Get(0);
      if (!m_context.HelperCheckEdge(Keynodes::op_waitReturn, wait_operator, ScType::EdgeAccessConstPosPerm)
          || !m_context.HelperCheckEdge(Keynodes::active_action, wait_operator, ScType::EdgeAccessConstPosPerm))
        continue;

      SCPOperator::FinishExecutionSuccessfully(m_context, wait_operator);
      break;
    }
  }

  return action.FinishSuccessfully();
}

ScAddr ASCPFinishedInterpretationActionProcessor::GetActionClass() const
{
  return Keynodes::action_interpret_finished_action;
}

ScAddr ASCPFinishedInterpretationActionProcessor::GetEventSubscriptionElement() const
{
  return Keynodes::action_finished;
}

bool ASCPFinishedInterpretationActionProcessor::CheckInitiationCondition(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event)
{
  return m_context.HelperCheckEdge(
      Keynodes::action_scp_interpretation_request, event.GetOtherElement(), ScType::EdgeAccessConstPosPerm);
}

}  // namespace scp
