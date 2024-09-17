/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpProcessInterpreter.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include <chrono>

namespace scp
{
ScResult ASCPProcessInterpreter::DoProgram(ScAction & action)
{
  auto const & startTime = std::chrono::high_resolution_clock::now();
  sc_uint32 const maxCustomerWaitingTime = action.GetMaxCustomerWaitingTime();
  auto const & [program, params] = action.GetArguments<2>();
  if (!program.IsValid() || !params.IsValid())
  {
    SC_AGENT_LOG_ERROR(
        "Action arguments expected to be valid, but got " << program.Hash() << " and " << params.Hash() << " instead");
    return action.FinishUnsuccessfully();
  }

  ScTemplate programTemplate;
  m_context.BuildTemplate(programTemplate, program);
  ScTemplateParams generationParams;

  ScAddr processNode;
  auto const & programKeyElementIterator = m_context.CreateIterator5(
      program,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeVar,
      ScType::EdgeAccessConstPosPerm,
      Keynodes::rrel_key_sc_element);
  if (programKeyElementIterator->IsValid() && programKeyElementIterator->Next())
    processNode = programKeyElementIterator->Get(2);
  else
  {
    SC_AGENT_LOG_ERROR("Cannot find key element of program " << program.Hash());
    return action.FinishUnsuccessfully();
  }

  auto const & processParametersIterator = m_context.CreateIterator5(
      processNode, ScType::EdgeAccessVarPosPerm, ScType::Unknown, ScType::EdgeAccessConstPosPerm, program);

  //! TODO Make all sc-links constant to allow using constant sc-links within scp-program code
  while (processParametersIterator->Next())
  {
    ScAddr order;
    if (Utils::resolveOrderRoleRelation(m_context, processParametersIterator->Get(1), order))
    {
      auto const & parametersIterator = m_context.CreateIterator5(
          params, ScType::EdgeAccessConstPosPerm, ScType::Unknown, ScType::EdgeAccessConstPosPerm, order);
      if (!parametersIterator->Next())
      {
#ifdef SCP_DEBUG
        Utils::logSCPError(m_context, "Missed scp-program parameter", order);
#endif
        continue;
      }
      generationParams.Add(processParametersIterator->Get(2), parametersIterator->Get(2));
    }
  }

  ScTemplateGenResult result;
  m_context.GenerateByTemplate(programTemplate, result, generationParams);

  ScAddr constProcessNode = result[processNode];
  auto const & generatedProcessDecompositionIterator = m_context.CreateIterator5(
      ScType::NodeConst,
      ScType::EdgeDCommonConst,
      constProcessNode,
      ScType::EdgeAccessConstPosPerm,
      Keynodes::nrel_decomposition_of_action);
  if (generatedProcessDecompositionIterator->Next())
  {
    ScIterator5Ptr firstOperatorIterator = m_context.CreateIterator5(
        generatedProcessDecompositionIterator->Get(0),
        ScType::EdgeAccessConstPosPerm,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        Keynodes::rrel_1);
    if (firstOperatorIterator->Next())
    {
      m_context.GenerateConnector(
          ScType::EdgeAccessConstPosPerm, Keynodes::active_action, firstOperatorIterator->Get(2));
      auto const & sub = m_context.CreateElementaryEventSubscription<ScEventBeforeEraseElement>(
          constProcessNode,
          [](auto const & otherEvent)
          {
            SC_LOG_ERROR("\n\n" << otherEvent.GetSubscriptionElement().Hash() << " was erased");
          });
      auto const & waiter =
          m_context.CreateConditionWaiter<ScEventAfterGenerateIncomingArc<ScType::EdgeAccessConstPosPerm>>(
              constProcessNode,
              [](auto const & otherEvent)
              {
                return otherEvent.GetArcSourceElement() == Keynodes::action_finished;
              });
      if (maxCustomerWaitingTime == 0)
        waiter->Wait();
      else
      {
        auto const & timeFromStart = scp::Utils::GetTimeFromStart(startTime);
        if (timeFromStart < maxCustomerWaitingTime)
          waiter->Wait(maxCustomerWaitingTime - timeFromStart);
        else
        {
          SC_AGENT_LOG_WARNING(
              "Max customer waiting time" << maxCustomerWaitingTime << " has expired before action "
                                          << constProcessNode.Hash() << " was initiated because" << timeFromStart
                                          << " ms have passed");
          waiter->Wait();
        }
      }
    }
    else
    {
#ifdef SCP_DEBUG
      Utils::logSCPError(m_context, "Missed initial scp-operator", program);
#endif
      m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, Keynodes::action_finished, constProcessNode);
    }
  }
  else
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_context, "Missed scp-process decomposition", program);
#endif
    m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, Keynodes::action_finished, constProcessNode);
  }
  return action.FinishSuccessfully();
}

ScAddr ASCPProcessInterpreter::GetActionClass() const
{
  return Keynodes::action_interpret_process;
}

bool ASCPProcessInterpreter::CheckInitiationCondition(ScActionInitiatedEvent const & event)
{
  ScAddr const & action = event.GetOtherElement();
  return m_context.CheckConnector(Keynodes::action_scp_interpretation_request, action, ScType::EdgeAccessConstPosPerm);
}
}  // namespace scp
