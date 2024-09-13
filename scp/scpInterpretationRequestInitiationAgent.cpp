#include "scpInterpretationRequestInitiationAgent.hpp"

#include "scpKeynodes.hpp"

#include <chrono>

namespace scp
{
ScResult SCPInterpretationRequestInitiationAgent::DoProgram(ScElementaryEvent const & event, ScAction & action)
{
  auto const & startTime = std::chrono::high_resolution_clock::now();
  ScAddr const & maxCustomerWaitingTimeLink = action.GetMaxCustomerWaitingTime();
  sc_uint32 maxCustomerWaitingTime = 0;
  if (m_context.IsElement(maxCustomerWaitingTimeLink))
    m_context.GetLinkContent(maxCustomerWaitingTimeLink, maxCustomerWaitingTime);

  ScAddr const & agentProgram = GetAgentProgram();
  if (!m_context.IsElement(agentProgram))
  {
    SC_AGENT_LOG_ERROR("Agent program is not valid");
    action.FinishUnsuccessfully();
  }

  ScAddr const & scpParams = m_context.CreateNode(ScType::NodeConst);
  ScAddr const & firstArgumentArc = m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, scpParams, agentProgram);
  m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::rrel_1, firstArgumentArc);

  ScAddr const & secondArgumentArc =
      m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, scpParams, event.GetConnector());
  m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::rrel_2, secondArgumentArc);

  ScAction scpAction = m_context.GenerateAction(Keynodes::action_scp_interpretation_request);
  scpAction.SetArguments(agentProgram, scpParams);

  ScAddr const & authorArc = m_context.CreateEdge(ScType::EdgeDCommonConst, scpAction, Keynodes::abstract_scp_machine);
  m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::nrel_authors, authorArc);

  if (maxCustomerWaitingTime == 0)
    scpAction.InitiateAndWait();
  else
  {
    auto const & timeFromStart = GetTimeFromStart(startTime);
    if (timeFromStart < maxCustomerWaitingTime)
      scpAction.InitiateAndWait(maxCustomerWaitingTime - timeFromStart);
    else
    {
      SC_AGENT_LOG_WARNING(
          "Max customer waiting time"
          << maxCustomerWaitingTime
          << " has expired before action of class `action_scp_interpretation_request` was initiated because"
          << timeFromStart << " ms have passed");
      scpAction.InitiateAndWait();
    }
  }
  if (scpAction.IsFinishedSuccessfully())
  {
    ScAddr const & result = scpAction.GetResult();
    if (!m_context.IsElement(result))
    {
      return action.FinishUnsuccessfully();
    }
    auto const & waiter =
        m_context.GenerateConditionWaiter<ScEventAfterGenerateIncomingArc<ScType::EdgeAccessConstPosPerm>>(
            result,
            [](ScEventAfterGenerateIncomingArc<ScType::EdgeAccessConstPosPerm> const & subscribedEvent)
            {
              return subscribedEvent.GetArcSourceElement() == Keynodes::action_finished;
            });
    if (maxCustomerWaitingTime == 0)
      waiter->Wait();
    else
    {
      auto const & timeFromStart = GetTimeFromStart(startTime);
      if (timeFromStart < maxCustomerWaitingTime)
        waiter->Wait(maxCustomerWaitingTime - timeFromStart);
      else
      {
        SC_AGENT_LOG_WARNING(
            "Max customer waiting time" << maxCustomerWaitingTime
                                        << " has expired before wait for scp interpretation to finish because"
                                        << timeFromStart << " ms have passed");
        waiter->Wait();
      }
    }
  }

  return action.FinishSuccessfully();
}

int64_t SCPInterpretationRequestInitiationAgent::GetTimeFromStart(
    std::chrono::time_point<std::chrono::high_resolution_clock> const & startTime)
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime)
      .count();
}

ScAddr SCPInterpretationRequestInitiationAgent::GetAgentProgram() const
{
  if (!m_context.IsElement(m_agentImplementationAddr))
  {
    SC_AGENT_LOG_ERROR("Agent implementation node is not valid");
    return ScAddr::Empty;
  }

  ScAddr agentProgram;
  auto const & programsTupleIterator = m_context.Iterator5(
      ScType::NodeConst,
      ScType::EdgeDCommonConst,
      m_agentImplementationAddr,
      ScType::EdgeAccessConstPosPerm,
      Keynodes::nrel_sc_agent_program);
  while (programsTupleIterator->Next())
  {
    ScIterator3Ptr programsIterator =
        m_context.Iterator3(programsTupleIterator->Get(0), ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (programsIterator->Next())
    {
      if (m_context.HelperCheckEdge(
              Keynodes::agent_scp_program, programsIterator->Get(2), ScType::EdgeAccessConstPosPerm))
      {
        agentProgram = programsIterator->Get(2);
        break;
      }
    }
  }
  if (!agentProgram.IsValid())
  {
    SC_AGENT_LOG_ERROR("Not found program for sc-agent `" << m_context.HelperGetSystemIdtf(GetAbstractAgent()) << "`");
    return ScAddr::Empty;
  }

  // Old SCP program check
  auto const & programKeyElementIterator = m_context.Iterator5(
      agentProgram,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeVar,
      ScType::EdgeAccessConstPosPerm,
      Keynodes::rrel_key_sc_element);
  if (!programKeyElementIterator->Next())
  {
    SC_AGENT_LOG_ERROR(
        "Not found process variable in program for sc-agent `" << m_context.HelperGetSystemIdtf(GetAbstractAgent())
                                                               << "`");
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
