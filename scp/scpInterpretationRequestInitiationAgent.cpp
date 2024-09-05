#include "scpInterpretationRequestInitiationAgent.hpp"

#include "scpKeynodes.hpp"

namespace scp
{
ScResult SCPInterpretationRequestInitiationAgent::DoProgram(ScElementaryEvent const & event, ScAction & action)
{
  ScAddr const & agentProgram = GetScAgentProgram();
  if (!m_context.IsElement(agentProgram))
    action.FinishUnsuccessfully();

  ScAddr const & scpParams = m_context.CreateNode(ScType::NodeConst);
  ScAddr const & firstArgumentArc = m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, scpParams, agentProgram);
  m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::rrel_1, firstArgumentArc);

  ScAddr const & secondArgumentArc = m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, scpParams, event.GetConnector());
  m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::rrel_2, secondArgumentArc);

  ScAction scpAction = m_context.GenerateAction(Keynodes::action_scp_interpretation_request);
  scpAction.SetArguments(agentProgram, scpParams);

  ScAddr const & authorArc = m_context.CreateEdge(ScType::EdgeDCommonConst, scpAction, Keynodes::abstract_scp_machine);
  m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::nrel_authors, authorArc);

  scpAction.Initiate();

  return action.FinishSuccessfully();
}

ScAddr SCPInterpretationRequestInitiationAgent::GetScAgentProgram() const
{
  if (!m_context.IsElement(m_agentImplementationAddr))
  {
    SC_AGENT_LOG_ERROR("Agent implementation node is not valid");
    return ScAddr::Empty;
  }

  ScAddr agentProgram;
  auto const & programsTupleIterator = m_context.Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, m_agentImplementationAddr, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_sc_agent_program);
  while (programsTupleIterator->Next())
  {
    ScIterator3Ptr programsIterator = m_context.Iterator3(programsTupleIterator->Get(0), ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (programsIterator->Next())
    {
      if (m_context.HelperCheckEdge(Keynodes::agent_scp_program, programsIterator->Get(2), ScType::EdgeAccessConstPosPerm))
      {
        agentProgram = programsIterator->Get(2);
        break;
      }
    }
  }
  if (!agentProgram.IsValid())
  {
    SC_AGENT_LOG_ERROR("Not found program for sc-agent \"" << m_context.HelperGetSystemIdtf(GetAbstractAgent()) << "\"");
    return ScAddr::Empty;
  }

  // Old SCP program check
  auto const & programKeyElementIterator = m_context.Iterator5(agentProgram, ScType::EdgeAccessConstPosPerm, ScType::NodeVar, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_key_sc_element);
  if (!programKeyElementIterator->Next())
  {
    SC_AGENT_LOG_ERROR("Not found process variable in program for sc-agent \"" << m_context.HelperGetSystemIdtf(GetAbstractAgent()) << "\"");
    return ScAddr::Empty;
  }
  return agentProgram;
}

bool SCPInterpretationRequestInitiationAgent::CheckInitiationCondition(ScElementaryEvent const & event)
{
  ScAddr const & action = event.GetOtherElement();
  auto const & actionAuthorIterator = m_context.Iterator5(
      action,
      ScType::EdgeDCommonConst,
      Keynodes::abstract_scp_machine,
      ScType::EdgeAccessConstPosPerm,
      Keynodes::nrel_authors);
  if (actionAuthorIterator->Next())
      return false;
  if (!m_context.HelperCheckEdge(GetActionClass(), action, ScType::EdgeAccessConstPosPerm))
    return false;
  return true;
}
}  // namespace scp
