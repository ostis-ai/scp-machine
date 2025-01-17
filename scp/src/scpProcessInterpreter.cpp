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
  auto const & [program, paramsForSubstitution] = action.GetArguments<2>();
  if (!program.IsValid() || !paramsForSubstitution.IsValid())
  {
    m_logger.Error(
        "Action arguments expected to be valid, but got ",
        program.Hash(),
        " and ",
        paramsForSubstitution.Hash(),
        " instead");
    return action.FinishUnsuccessfully();
  }

  ScTemplate programTemplate;
  m_context.BuildTemplate(programTemplate, program);

  ScAddr const & variableProcessNode = getProgramProcess(program);
  if (!variableProcessNode.IsValid())
  {
    m_logger.Error("Cannot find key element of program ", program.Hash());
    return action.FinishUnsuccessfully();
  }

  ScTemplateParams const & generationParams =
      getProgramGenerationParams(program, paramsForSubstitution, variableProcessNode);

  ScTemplateGenResult result;
  m_context.GenerateByTemplate(programTemplate, result, generationParams);

  ScAddr const & constProcessNode = result[variableProcessNode];
  initiateFirstProcessOperatorAndWaitFinish(startTime, maxCustomerWaitingTime, constProcessNode, program);
  return action.FinishSuccessfully();
}

ScAddr ASCPProcessInterpreter::getProgramProcess(ScAddr const & program) const
{
  ScAddr processNode;
  auto const & programKeyElementIterator = m_context.CreateIterator5(
      program, ScType::ConstPermPosArc, ScType::VarNode, ScType::ConstPermPosArc, Keynodes::rrel_key_sc_element);
  if (programKeyElementIterator->IsValid() && programKeyElementIterator->Next())
    processNode = programKeyElementIterator->Get(2);
  return processNode;
}

ScTemplateParams ASCPProcessInterpreter::getProgramGenerationParams(
    ScAddr const & program,
    ScAddr const & paramsForSubstitution,
    ScAddr const & processNode) const
{
  ScTemplateParams generationParams;
  auto const & processParametersIterator =
      m_context.CreateIterator5(processNode, ScType::VarPermPosArc, ScType::Unknown, ScType::ConstPermPosArc, program);

  //! TODO Make all sc-links constant to allow using constant sc-links within scp-program code
  while (processParametersIterator->Next())
  {
    ScAddr order;
    if (Utils::resolveOrderRoleRelation(m_context, processParametersIterator->Get(1), order))
    {
      auto const & parametersIterator = m_context.CreateIterator5(
          paramsForSubstitution, ScType::ConstPermPosArc, ScType::Unknown, ScType::ConstPermPosArc, order);
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
  return generationParams;
}

void ASCPProcessInterpreter::initiateFirstProcessOperatorAndWaitFinish(
    std::chrono::time_point<std::chrono::high_resolution_clock> const & startTime,
    sc_uint32 const maxCustomerWaitingTime,
    ScAddr const & processNode,
    ScAddr const & programNode) const
{
  auto const & generatedProcessDecompositionIterator = m_context.CreateIterator5(
      ScType::ConstNode,
      ScType::ConstCommonArc,
      processNode,
      ScType::ConstPermPosArc,
      Keynodes::nrel_decomposition_of_action);
  if (generatedProcessDecompositionIterator->Next())
  {
    ScIterator5Ptr firstOperatorIterator = m_context.CreateIterator5(
        generatedProcessDecompositionIterator->Get(0),
        ScType::ConstPermPosArc,
        ScType::ConstNode,
        ScType::ConstPermPosArc,
        Keynodes::rrel_1);
    if (firstOperatorIterator->Next())
    {
      m_context.GenerateConnector(ScType::ConstPermPosArc, Keynodes::active_action, firstOperatorIterator->Get(2));
      waitProcessFinish(startTime, maxCustomerWaitingTime, processNode);
    }
    else
    {
#ifdef SCP_DEBUG
      Utils::logSCPError(m_context, "Missed initial scp-operator", programNode);
#endif
      m_context.GenerateConnector(ScType::ConstPermPosArc, Keynodes::action_finished, processNode);
    }
  }
  else
  {
#ifdef SCP_DEBUG
    Utils::logSCPError(m_context, "Missed scp-process decomposition", programNode);
#endif
    m_context.GenerateConnector(ScType::ConstPermPosArc, Keynodes::action_finished, processNode);
  }
}

void ASCPProcessInterpreter::waitProcessFinish(
    std::chrono::time_point<std::chrono::high_resolution_clock> const & startTime,
    sc_uint32 const maxCustomerWaitingTime,
    ScAddr const & processNode) const
{
  auto const & waiter = m_context.CreateConditionWaiter<ScEventAfterGenerateIncomingArc<ScType::ConstPermPosArc>>(
      processNode,
      [](auto const & otherEvent)
      {
        return otherEvent.GetArcSourceElement() == Keynodes::action_finished;
      });
  if (maxCustomerWaitingTime == 0)
    waiter->Wait();
  else
  {
    auto const & timeFromStart = Utils::GetTimeFromStart(startTime);
    if (timeFromStart < maxCustomerWaitingTime)
      waiter->Wait(maxCustomerWaitingTime - timeFromStart);
    else
    {
      m_logger.Error(
          "Max customer waiting time",
          maxCustomerWaitingTime,
          " has expired before action ",
          processNode.Hash(),
          " was initiated because",
          timeFromStart,
          " ms have passed");
      waiter->Wait();
    }
  }
}

ScAddr ASCPProcessInterpreter::GetActionClass() const
{
  return Keynodes::action_scp_interpretation_request;
}

bool ASCPProcessInterpreter::CheckInitiationCondition(ScActionInitiatedEvent const & event)
{
  ScAddr const & action = event.GetOtherElement();
  return m_context.CheckConnector(Keynodes::action_scp_interpretation_request, action, ScType::ConstPermPosArc);
}
}  // namespace scp
