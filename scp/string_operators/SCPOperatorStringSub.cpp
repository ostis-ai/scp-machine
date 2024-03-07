/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "SCPOperatorStringSub.hpp"
#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_stream.hpp"
#include <sc-memory/sc_link.hpp>
#include <iostream>
#include <cstring>

using namespace std;

namespace scp
{

SCPOperatorStringSub::SCPOperatorStringSub(ScMemoryContext &ctx, ScAddr addr): SCPOperatorElStr3(ctx, addr)
{
}

string SCPOperatorStringSub::GetTypeName()
{
    return "stringSub";
}

sc_result SCPOperatorStringSub::Parse()
{
    return SCPOperatorElStr3::Parse();
}

sc_result SCPOperatorStringSub::Execute()
{
    if (SC_RESULT_OK != ResetValues())
        return SC_RESULT_ERROR;

    if (!(operands[1]->IsFixed() && operands[2]->IsFixed()))
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(m_memoryCtx, "Both operands must have FIXED modifier", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }
    if (!operands[1]->GetValue().IsValid())
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(m_memoryCtx, "Operand 1 has modifier FIXED, but has no value", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    if (!operands[2]->GetValue().IsValid())
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(m_memoryCtx, "Operand 2 has modifier FIXED, but has no value", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }


        string str1 = Utils::scLinkGetString(m_memoryCtx, operands[1]->GetValue());
        string str2 = Utils::scLinkGetString(m_memoryCtx, operands[2]->GetValue());


        size_t start_pos = str1.find(str2);
        if(start_pos == std::string::npos)
        {
            FinishExecutionUnsuccessfully();
        }
        else
        {
            ScStreamPtr streamPtr = Utils::StreamFromString(to_string((start_pos)));
            ScAddr answerLink =m_memoryCtx.CreateLink();

           m_memoryCtx.SetLinkContent(answerLink, streamPtr);

            Utils::printOperatorAnswer(m_memoryCtx, operands[0], answerLink);

            FinishExecutionSuccessfully();

        }

    return SC_RESULT_OK;
}

}
