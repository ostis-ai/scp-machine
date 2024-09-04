/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpProgramExecutionSyncronizer.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include <iostream>

namespace scp {
ScResult ASCPProgramExecutionSyncronizer::DoProgram(ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event, ScAction & action)
{
    if (!event.GetArc().IsValid())
        return action.FinishUnsuccessfully();

    ScAddr scp_operator =m_context.GetEdgeTarget(event.GetArc());

    if (m_context.HelperCheckEdge(Keynodes::action_finished_with_error, scp_operator, ScType::EdgeAccessConstPosPerm))
    {
        InitOperatorsByRelation(scp_operator, Keynodes::nrel_error);
        return action.FinishSuccessfully();
    }
    if (m_context.HelperCheckEdge(Keynodes::action_finished_successfully, scp_operator, ScType::EdgeAccessConstPosPerm))
    {
        InitOperatorsByRelation(scp_operator, Keynodes::nrel_then);
        InitOperatorsByRelation(scp_operator, Keynodes::nrel_goto);
        return action.FinishSuccessfully();
    }
    if (m_context.HelperCheckEdge(Keynodes::action_finished_unsuccessfully, scp_operator, ScType::EdgeAccessConstPosPerm))
    {
        InitOperatorsByRelation(scp_operator, Keynodes::nrel_else);
        InitOperatorsByRelation(scp_operator, Keynodes::nrel_goto);
        return action.FinishSuccessfully();
    }

    return action.FinishSuccessfully();
}

ScAddr ASCPProgramExecutionSyncronizer::GetActionClass() const
{
//todo(codegen-removal): replace action with your action class
  return ScKeynodes::action;
}

ScAddr ASCPProgramExecutionSyncronizer::GetEventSubscriptionElement() const
{
  return Keynodes::action_finished;
}

void ASCPProgramExecutionSyncronizer::InitOperatorsByRelation(ScAddr &scp_operator, ScAddr const & relation)
{
    ScIterator5Ptr iter_error =m_context.Iterator5(scp_operator, ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, relation);
    while (iter_error->Next())
    {
        ScAddr next_op = iter_error->Get(2);
       m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::active_action, next_op);
    }
}

}
