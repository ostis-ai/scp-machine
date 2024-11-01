/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpProcessDestroyer.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include <iostream>

namespace scp
{
ScResult ASCPProcessDestroyer::DoProgram(
    ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc> const & event,
    ScAction & action)
{
  ScAddr process = event.GetOtherElement();

  ScAddr decomp_node;
  ScIterator5Ptr it_temp = m_context.CreateIterator5(
      ScType::ConstNode,
      ScType::ConstCommonArc,
      process,
      ScType::ConstPermPosArc,
      Keynodes::nrel_decomposition_of_action);
  if (it_temp->Next())
    decomp_node = it_temp->Get(0);
  else
    return action.FinishUnsuccessfully();

  ScIterator3Ptr it_oper = m_context.CreateIterator3(decomp_node, ScType::ConstPermPosArc, ScType::ConstNode);
  while (it_oper->Next())
  {
    ScAddr curr_operator = it_oper->Get(2);

    if (m_context.CheckConnector(Keynodes::op_sys_search, curr_operator, ScType::ConstPermPosArc)
        || m_context.CheckConnector(Keynodes::op_sys_gen, curr_operator, ScType::ConstPermPosArc))
    {
      ScIterator5Ptr it_operand = m_context.CreateIterator5(
          curr_operator, ScType::ConstPermPosArc, ScType::ConstNode, ScType::ConstPermPosArc, Keynodes::rrel_scp_const);
      while (it_operand->Next())
      {
        if (!(m_context.CheckConnector(Keynodes::rrel_2, it_operand->Get(1), ScType::ConstPermPosArc)
              || m_context.CheckConnector(Keynodes::rrel_3, it_operand->Get(1), ScType::ConstPermPosArc)))
          continue;

        ScAddr curr_operand = it_operand->Get(2);
        ScIterator3Ptr it_pairs = m_context.CreateIterator3(curr_operand, ScType::ConstPermPosArc, ScType::Unknown);
        while (it_pairs->Next())
        {
          ScAddr curr_pair = it_pairs->Get(2);
          // todo(kilativ-dotcom): review commented method, if it is uncommented then sometimes
          // scpGenOperatorsTest.ComplexAgentsChain fails
          //          deleteSCPVarsSet(curr_pair, process);
          m_context.EraseElement(curr_pair);
        }
        m_context.EraseElement(curr_operand);
      }
    }
    if (m_context.CheckConnector(Keynodes::op_call, curr_operator, ScType::ConstPermPosArc))
    {
      ScIterator5Ptr it_operand = m_context.CreateIterator5(
          curr_operator, ScType::ConstPermPosArc, ScType::ConstNode, ScType::ConstPermPosArc, Keynodes::rrel_2);
      if (it_operand->Next())
      {
        if (m_context.CheckConnector(Keynodes::rrel_scp_const, it_operand->Get(1), ScType::ConstPermPosArc))
        {
          ScAddr curr_operand = it_operand->Get(2);
          //          deleteSCPVarsSet(curr_operand, process);
          m_context.EraseElement(curr_operand);
        }
      }
    }

    //    deleteSCPVarsSet(curr_operator, process);
    m_context.EraseElement(curr_operator);
  }

  m_context.EraseElement(decomp_node);
  m_context.EraseElement(process);

  return action.FinishSuccessfully();
}

ScAddr ASCPProcessDestroyer::GetActionClass() const
{
  return Keynodes::action_destroy_process;
}

ScAddr ASCPProcessDestroyer::GetEventSubscriptionElement() const
{
  return Keynodes::action_finished;
}

void ASCPProcessDestroyer::deleteSCPVarsSet(ScAddr & setAddr, ScAddr & processAddr)
{
  ScAddrVector elems;
  ScIterator5Ptr it_operand = m_context.CreateIterator5(
      setAddr, ScType::ConstPermPosArc, ScType::ConstNode, ScType::ConstPermPosArc, Keynodes::rrel_scp_var);
  while (it_operand->Next())
  {
    ScAddr elem = it_operand->Get(2);
    ScIterator5Ptr it_params = m_context.CreateIterator5(
        processAddr, ScType::ConstPermPosArc, elem, ScType::ConstPermPosArc, Keynodes::rrel_out);
    if (!it_params->Next())
    {
      elems.push_back(elem);
    }
  }

  for (auto const & elem : elems)
  {
    m_context.EraseElement(elem);
  }
}

bool ASCPProcessDestroyer::CheckInitiationCondition(
    ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc> const & event)
{
  return m_context
      .CreateIterator5(
          ScType::ConstNode,
          ScType::ConstCommonArc,
          event.GetOtherElement(),
          ScType::ConstPermPosArc,
          Keynodes::nrel_decomposition_of_action)
      ->Next();
}

}  // namespace scp
