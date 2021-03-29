/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "SCPOperatorContStringConcat.hpp"
#include "sc-memory/sc-memory/sc_memory.hpp"
#include "sc-memory/sc-memory/sc_stream.hpp"
#include <sc-memory/sc-memory/sc_link.hpp>
#include <iostream>
#include <cstring>

using namespace std;

namespace scp
{

SCPOperatorContStringConcat::SCPOperatorContStringConcat(const std::unique_ptr<ScMemoryContext> &ctx, ScAddr addr): SCPOperatorElStr3(ctx, addr)
{
}

string SCPOperatorContStringConcat::GetTypeName()
{
    return "contStringConcat";
}

sc_result SCPOperatorContStringConcat::Parse()
{
    return SCPOperatorElStr3::Parse();
}

sc_result SCPOperatorContStringConcat::Execute()
{
    if (SC_RESULT_OK != ResetValues())
        return SC_RESULT_ERROR;

    if (!(operands[1]->IsFixed() && operands[2]->IsFixed()))
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(ms_context, "Both operands must have FIXED modifier", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }
    if (!operands[1]->GetValue().IsValid())
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(ms_context, "Operand 1 has modifier FIXED, but has no value", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    if (!operands[2]->GetValue().IsValid())
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(ms_context, "Operand 2 has modifier FIXED, but has no value", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }


        string str1 = Utils::scLinkGetString(ms_context, operands[1]->GetValue());
        string str2 = Utils::scLinkGetString(ms_context, operands[2]->GetValue());


        string str3 = str1 + str2;

            ScStreamPtr streamPtr = Utils::StreamFromString(str3);
            ScAddr answerLink = ms_context->CreateLink();

            ms_context->SetLinkContent(answerLink, streamPtr);

            Utils::printOperatorAnswer(ms_context, operands[0], answerLink);

            FinishExecutionSuccessfully();
    return SC_RESULT_OK;
}

}
