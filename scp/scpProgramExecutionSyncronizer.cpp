/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpProgramExecutionSyncronizer.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include <iostream>

namespace scp
{
ScResult ASCPProgramExecutionSyncronizer::DoProgram(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event,
    ScAction & action)
{
  ScAddr const & scpOperatorAddr = event.GetOtherElement();

  if (m_context.HelperCheckEdge(Keynodes::action_finished_with_error, scpOperatorAddr, ScType::EdgeAccessConstPosPerm))
  {
    InitOperatorsByRelation(scpOperatorAddr, Keynodes::nrel_error);
    return action.FinishSuccessfully();
  }
  if (m_context.HelperCheckEdge(
          Keynodes::action_finished_successfully, scpOperatorAddr, ScType::EdgeAccessConstPosPerm))
  {
    InitOperatorsByRelation(scpOperatorAddr, Keynodes::nrel_then);
    InitOperatorsByRelation(scpOperatorAddr, Keynodes::nrel_goto);
    return action.FinishSuccessfully();
  }
  if (m_context.HelperCheckEdge(
          Keynodes::action_finished_unsuccessfully, scpOperatorAddr, ScType::EdgeAccessConstPosPerm))
  {
    InitOperatorsByRelation(scpOperatorAddr, Keynodes::nrel_else);
    InitOperatorsByRelation(scpOperatorAddr, Keynodes::nrel_goto);
    return action.FinishSuccessfully();
  }

  return action.FinishSuccessfully();
}

ScAddr ASCPProgramExecutionSyncronizer::GetActionClass() const
{
  return Keynodes::action_synchronize_program_execution;
}

ScAddr ASCPProgramExecutionSyncronizer::GetEventSubscriptionElement() const
{
  return Keynodes::action_finished;
}

void ASCPProgramExecutionSyncronizer::InitOperatorsByRelation(ScAddr const & scpOperatorAddr, ScAddr const & relation)
{
  ScIterator5Ptr iter_error = m_context.Iterator5(
      scpOperatorAddr, ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, relation);
  while (iter_error->Next())
  {
    ScAddr next_op = iter_error->Get(2);
    m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::active_action, next_op);
  }
}

bool ASCPProgramExecutionSyncronizer::CheckInitiationCondition(
    ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event)
{
  ScAddr const & scpOperatorAddr = event.GetOtherElement();

  ScAddr type;
  return Utils::resolveOperatorType(m_context, scpOperatorAddr, type);
}

}  // namespace scp
