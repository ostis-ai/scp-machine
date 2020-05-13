/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "SCPOperatorStringToUpperCase.hpp"
#include "sc-memory/cpp/sc_memory.hpp"
#include "sc-memory/cpp/sc_stream.hpp"
#include <sc-memory/cpp/sc_link.hpp>
#include <iostream>
#include <cstring>
#include <boost/algorithm/string.hpp>

using namespace std;

namespace scp
{

SCPOperatorStringToUpperCase::SCPOperatorStringToUpperCase(const std::unique_ptr<ScMemoryContext> &ctx, ScAddr addr): SCPOperatorElStr2(ctx, addr)
{
}

string SCPOperatorStringToUpperCase::GetTypeName()
{
    return "stringToUpperCase";
}

sc_result SCPOperatorStringToUpperCase::Parse()
{
    return SCPOperatorElStr2::Parse();
}

sc_result SCPOperatorStringToUpperCase::Execute()
{
    if (SC_RESULT_OK != ResetValues())
        return SC_RESULT_ERROR;

    if (!operands[1]->IsFixed())
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(ms_context, "Operand must have FIXED modifier", addr);
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


        string str1 = Utils::scLinkGetString(ms_context, operands[1]->GetValue());
        if(str1=="")
        {
            cout << "Link content is empty!" << endl;
            FinishExecutionUnsuccessfully();
            return SC_RESULT_OK;
        }

            ScStreamPtr streamPtr = Utils::StreamFromString(boost::to_upper_copy(str1));
            ScAddr answerLink = ms_context->CreateLink();

            ms_context->SetLinkContent(answerLink, *streamPtr);
            ms_context->HelperSetSystemIdtf("stringToUpperCaseAnswer", answerLink);

            Utils::printOperatorAnswer(ms_context, operands[0], answerLink);


            FinishExecutionSuccessfully();

    return SC_RESULT_OK;
}

}
