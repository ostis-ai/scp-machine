/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpUtils.hpp"

#include "scpOperatorInterpreter.hpp"

namespace scp
{
ScResult ASCPOperatorInterpreter::DoProgram(
    ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc> const & event,
    ScAction & action)
{
  ScAddr const & scpOperatorAddr = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scpOperatorAddr, type))
  {
    m_logger.Error("Cannot resolve operator type for ", scpOperatorAddr.Hash());
    return action.FinishUnsuccessfully();
  }

  std::unique_ptr<SCPOperator> scpOperator = nullptr;

  auto const & supportedOperators = getSupportedOperators();
  auto const & pair = supportedOperators.find(type);
  if (pair != supportedOperators.cend())
    scpOperator = pair->second(m_context, scpOperatorAddr);

  if (scpOperator == nullptr)
  {
    m_logger.Error("Cannot create operator from ", scpOperatorAddr.Hash());
    return action.FinishUnsuccessfully();
  }

#ifdef SCP_DEBUG
  std::cout << scpOperator->GetTypeName() << std::endl;
#endif
  sc_result parseResult = scpOperator->Parse();
  if (parseResult != SC_RESULT_OK)
  {
    m_logger.Error("Cannot parse operator ", scpOperatorAddr.Hash());
    return action.FinishUnsuccessfully();
  }
  else
  {
    sc_result executeResult;
    executeResult = scpOperator->Execute();
    return (executeResult == SC_RESULT_OK) ? action.FinishSuccessfully() : action.FinishUnsuccessfully();
  }
}

bool ASCPOperatorInterpreter::CheckInitiationCondition(
    ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc> const & event)
{
  ScAddr const & scpOperatorAddr = event.GetOtherElement();

  ScAddr type;
  if (!Utils::resolveOperatorType(m_context, scpOperatorAddr, type))
    return false;
  return getSupportedOperators().count(type);
}

ScAddr ASCPOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}
}  // namespace scp
