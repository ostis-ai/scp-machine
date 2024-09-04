/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpSearchOperatorInterpreter.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "search_operators/SCPOperatorSearchElStr3.hpp"
#include "search_operators/SCPOperatorSearchElStr5.hpp"
#include "search_operators/SCPOperatorSearchSetStr3.hpp"
#include "search_operators/SCPOperatorSearchSetStr5.hpp"
#include "search_operators/SCPOperatorSysSearch.hpp"

#include <iostream>

namespace scp {
ScResult ASCPSearchOperatorInterpreter::DoProgram(ScEventAfterGenerateOutgoingArc<ScType::EdgeAccessConstPosPerm> const & event, ScAction & action)
{
    if (!event.GetArc().IsValid())
        return action.FinishUnsuccessfully();

    ScAddr scp_operator = event.GetOtherElement();

    ScAddr type;
    if (SC_TRUE != Utils::resolveOperatorType(m_context, scp_operator, type))
        return action.FinishUnsuccessfully();

    SCPOperator* oper = nullptr;
    if (type == Keynodes::op_searchElStr3)
    {
        oper = new SCPOperatorSearchElStr3(m_context, scp_operator);
    }
    if (type == Keynodes::op_searchElStr5)
    {
        oper = new SCPOperatorSearchElStr5(m_context, scp_operator);
    }
    if (type == Keynodes::op_searchSetStr3)
    {
        oper = new SCPOperatorSearchSetStr3(m_context, scp_operator);
    }
    if (type == Keynodes::op_searchSetStr5)
    {
        oper = new SCPOperatorSearchSetStr5(m_context, scp_operator);
    }
    if (type == Keynodes::op_sys_search)
    {
        oper = new SCPOperatorSysSearch(m_context, scp_operator);
    }
    if (oper == nullptr)
        return action.FinishUnsuccessfully();

#ifdef SCP_DEBUG
    std::cout << oper->GetTypeName() << std::endl;
#endif
    sc_result parse_result = oper->Parse();
    if (parse_result != SC_RESULT_OK)
    {
        delete oper;
        return action.FinishUnsuccessfully();
    }
    else
    {
        sc_result execute_result;
        execute_result = oper->Execute();
        delete oper;
        return (execute_result == SC_RESULT_OK) ? action.FinishSuccessfully() : action.FinishUnsuccessfully();
    }
}

ScAddr ASCPSearchOperatorInterpreter::GetActionClass() const
{
//todo(codegen-removal): replace action with your action class
  return ScKeynodes::action;
}

ScAddr ASCPSearchOperatorInterpreter::GetEventSubscriptionElement() const
{
  return Keynodes::active_action;
}

}
