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
ScResult ASCPProcessDestroyer::DoProgram(ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event, ScAction & action)
{
    if (!event.GetArc().IsValid())
        return action.FinishUnsuccessfully();

    ScAddr process = event.GetOtherElement();

    ScAddr decomp_node;
    ScIterator5Ptr it_temp =m_context.Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, process, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_decomposition_of_action);
    if (it_temp->Next())
        decomp_node = it_temp->Get(0);
    else return action.FinishUnsuccessfully();

    ScIterator3Ptr it_oper =m_context.Iterator3(decomp_node, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (it_oper->Next())
    {
        ScAddr curr_operator = it_oper->Get(2);

        if (m_context.HelperCheckEdge(Keynodes::op_sys_search, curr_operator, ScType::EdgeAccessConstPosPerm)
                ||m_context.HelperCheckEdge(Keynodes::op_sys_gen, curr_operator, ScType::EdgeAccessConstPosPerm))
        {
            ScIterator5Ptr it_operand =m_context.Iterator5(curr_operator, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_scp_const);
            while (it_operand->Next())
            {
                if (!(m_context.HelperCheckEdge(Keynodes::rrel_2, it_operand->Get(1), ScType::EdgeAccessConstPosPerm)
                        ||m_context.HelperCheckEdge(Keynodes::rrel_3, it_operand->Get(1), ScType::EdgeAccessConstPosPerm)))
                    continue;

                ScAddr curr_operand = it_operand->Get(2);
                ScIterator3Ptr it_pairs =m_context.Iterator3(curr_operand, ScType::EdgeAccessConstPosPerm, ScType(0));
                while (it_pairs->Next())
                {
                    ScAddr curr_pair = it_pairs->Get(2);
                    deleteSCPVarsSet(curr_pair, process);
                   m_context.EraseElement(curr_pair);
                }
               m_context.EraseElement(curr_operand);
            }
        }
        if (m_context.HelperCheckEdge(Keynodes::op_call, curr_operator, ScType::EdgeAccessConstPosPerm))
        {
            ScIterator5Ptr it_operand =m_context.Iterator5(curr_operator, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_2);
            if (it_operand->Next())
            {
                if (m_context.HelperCheckEdge(Keynodes::rrel_scp_const, it_operand->Get(1), ScType::EdgeAccessConstPosPerm))
                {
                    ScAddr curr_operand = it_operand->Get(2);
                    deleteSCPVarsSet(curr_operand, process);
                   m_context.EraseElement(curr_operand);
                }
            }
        }

        deleteSCPVarsSet(curr_operator, process);
       m_context.EraseElement(curr_operator);
    }

   m_context.EraseElement(decomp_node);
   m_context.EraseElement(process);

    return action.FinishSuccessfully();
}

ScAddr ASCPProcessDestroyer::GetActionClass() const
{
  return Keynodes::scp_process;
}

ScAddr ASCPProcessDestroyer::GetEventSubscriptionElement() const
{
  return Keynodes::action_finished;
}

void ASCPProcessDestroyer::deleteSCPVarsSet(ScAddr & setAddr, ScAddr & processAddr)
{
    ScAddrVector elems;
    ScIterator5Ptr it_operand =m_context.Iterator5(setAddr, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_scp_var);
    while (it_operand->Next())
    {
        ScAddr elem = it_operand->Get(2);
        ScIterator5Ptr it_params =m_context.Iterator5(processAddr, ScType::EdgeAccessConstPosPerm, elem, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_out);
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

}
