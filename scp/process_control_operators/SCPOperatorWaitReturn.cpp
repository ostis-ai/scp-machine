/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "SCPOperatorWaitReturn.hpp"
#include "sc-memory/cpp/sc_memory.hpp"
#include "sc-memory/cpp/sc_wait.hpp"
#include <iostream>

//!TODO Eliminate time dependency
#define WAIT_TIMEOUT 59999

namespace scp {

//genEl
SCPOperatorWaitReturn::SCPOperatorWaitReturn(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr addr): SCPOperatorElStr1(ctx, addr)
{
}

std::string SCPOperatorWaitReturn::GetTypeName()
{
    return "waitReturn";
}

sc_result SCPOperatorWaitReturn::Parse()
{
    return SCPOperatorElStr1::Parse();
}

sc_result SCPOperatorWaitReturn::Execute()
{
    if (SC_RESULT_OK != ResetValues())
        return SC_RESULT_ERROR;

    if (!operands[0]->IsFixed())
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(ms_context, "Operand must have FIXED modifier", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }
    if (!operands[0]->GetValue().IsValid())
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(ms_context, "Operand has modifier FIXED, but has no value", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    auto check = [](ScAddr const & listenAddr,
                    ScAddr const & edgeAddr,
                    ScAddr const & otherAddr)
    {
        return otherAddr==Keynodes::question_finished;
    };

    //!TODO Fix first parameter pass
    ScWaitCondition<ScEventAddInputEdge> waiter((ScMemoryContext&)ms_context, operands[0]->GetValue(), SC_WAIT_CHECK(check));
    if (waiter.Wait(WAIT_TIMEOUT))
    {
        FinishExecutionSuccessfully();
    }
    else
    {
        FinishExecutionUnsuccessfully();
    }
    return SC_RESULT_OK;
}
}
