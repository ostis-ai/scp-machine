/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "SCPOperatorStringStartsWith.hpp"
#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_stream.hpp"
#include <sc-memory/sc_link.hpp>
#include <iostream>
#include <cstring>

using namespace std;

namespace scp
{

SCPOperatorStringStartsWith::SCPOperatorStringStartsWith(ScMemoryContext &ctx, ScAddr addr): SCPOperatorElStr2(ctx, addr)
{
}

string SCPOperatorStringStartsWith::GetTypeName()
{
    return "stringStartsWith";
}

sc_result SCPOperatorStringStartsWith::Parse()
{
    return SCPOperatorElStr2::Parse();
}

sc_result SCPOperatorStringStartsWith::Execute()
{
    if (SC_RESULT_OK != ResetValues())
        return SC_RESULT_ERROR;

    if (!(operands[0]->IsFixed() && operands[1]->IsFixed()))
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(m_memoryCtx, "Both operands must have FIXED modifier", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }
    if (!operands[0]->GetValue().IsValid())
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(m_memoryCtx, "Operand 1 has modifier FIXED, but has no value", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    if (!operands[1]->GetValue().IsValid())
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(m_memoryCtx, "Operand 2 has modifier FIXED, but has no value", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }


        string str1 = Utils::scLinkGetString(m_memoryCtx, operands[0]->GetValue());
        string str2 = Utils::scLinkGetString(m_memoryCtx, operands[1]->GetValue());

            size_t len2 = str2.length();

            if(strncmp(str1.c_str(), str2.c_str(), len2) == 0)
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
