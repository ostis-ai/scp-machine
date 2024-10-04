/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpHandlingEventThatInitiatesAgentSCPProgram.hpp"

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include <chrono>

namespace scp
{
ScResult ASCPHandlingEventThatInitiatesAgentSCPProgram::DoProgram(ScElementaryEvent const & event, ScAction & action)
{
  auto const & startTime = std::chrono::high_resolution_clock::now();
  sc_uint32 const maxCustomerWaitingTime = action.GetMaxCustomerWaitingTime();

  ScAddr const & agentProgram = GetAgentProgram();
  if (!m_context.IsElement(agentProgram))
  {
    SC_AGENT_LOG_ERROR("Agent program is not valid");
    action.FinishUnsuccessfully();
  }

  ScAddr const & scpParams = m_context.GenerateNode(ScType::ConstNode);
  ScAddr const & firstArgumentArc = m_context.GenerateConnector(ScType::ConstPermPosArc, scpParams, agentProgram);
  m_context.GenerateConnector(ScType::ConstPermPosArc, Keynodes::rrel_1, firstArgumentArc);

  ScAddr const & secondArgumentArc =
      m_context.GenerateConnector(ScType::ConstPermPosArc, scpParams, event.GetConnector());
  m_context.GenerateConnector(ScType::ConstPermPosArc, Keynodes::rrel_2, secondArgumentArc);

  ScAction scpAction = m_context.GenerateAction(Keynodes::action_scp_interpretation_request);
  scpAction.SetArguments(agentProgram, scpParams);

  ScAddr const & authorArc =
      m_context.GenerateConnector(ScType::ConstCommonArc, scpAction, Keynodes::abstract_scp_machine);
  m_context.GenerateConnector(ScType::ConstPermPosArc, Keynodes::nrel_authors, authorArc);

  if (maxCustomerWaitingTime == 0)
    scpAction.InitiateAndWait();
  else
  {
    auto const & timeFromStart = scp::Utils::GetTimeFromStart(startTime);
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
  auto const & resIt = m_context.CreateIterator5(
      action, ScType::ConstCommonArc, ScType::ConstNode, ScType::ConstPermPosArc, ScKeynodes::nrel_result);
  if (resIt->Next())
  {
    ScAddr const & actionResult = resIt->Get(2);
    action.SetResult(actionResult);
  }
  return action.FinishSuccessfully();
}

ScAddr ASCPHandlingEventThatInitiatesAgentSCPProgram::GetAgentProgram() const
{
  if (!m_context.IsElement(m_agentImplementationAddr))
  {
    SC_AGENT_LOG_ERROR("Agent implementation node is not valid");
    return ScAddr::Empty;
  }

  ScAddr agentProgram;
  auto const & programsTupleIterator = m_context.CreateIterator5(
      ScType::ConstNode,
      ScType::ConstCommonArc,
      m_agentImplementationAddr,
      ScType::ConstPermPosArc,
      Keynodes::nrel_sc_agent_program);
  while (programsTupleIterator->Next())
  {
    ScIterator3Ptr programsIterator =
        m_context.CreateIterator3(programsTupleIterator->Get(0), ScType::ConstPermPosArc, ScType::ConstNode);
    while (programsIterator->Next())
    {
      if (m_context.CheckConnector(Keynodes::agent_scp_program, programsIterator->Get(2), ScType::ConstPermPosArc))
      {
        agentProgram = programsIterator->Get(2);
        break;
      }
    }
  }
  if (!agentProgram.IsValid())
  {
    SC_AGENT_LOG_ERROR(
        "Not found program for sc-agent `" << m_context.GetElementSystemIdentifier(GetAbstractAgent()) << "`");
    return ScAddr::Empty;
  }

  // Old SCP program check
  auto const & programKeyElementIterator = m_context.CreateIterator5(
      agentProgram, ScType::ConstPermPosArc, ScType::VarNode, ScType::ConstPermPosArc, Keynodes::rrel_key_sc_element);
  if (!programKeyElementIterator->Next())
  {
    SC_AGENT_LOG_ERROR(
        "Not found process variable in program for sc-agent `"
        << m_context.GetElementSystemIdentifier(GetAbstractAgent()) << "`");
    return ScAddr::Empty;
  }
  return agentProgram;
}
}  // namespace scp
