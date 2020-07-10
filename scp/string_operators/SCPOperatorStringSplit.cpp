/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "SCPOperatorStringSplit.hpp"
#include "sc-memory/cpp/sc_memory.hpp"
#include "sc-memory/cpp/sc_stream.hpp"
#include <sc-memory/cpp/sc_link.hpp>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <iterator>

using namespace std;

namespace scp
{

SCPOperatorStringSplit::SCPOperatorStringSplit(const std::unique_ptr<ScMemoryContext> &ctx, ScAddr addr): SCPOperatorElStr3(ctx, addr)
{
}

string SCPOperatorStringSplit::GetTypeName()
{
    return "stringSplit";
}

sc_result SCPOperatorStringSplit::Parse()
{
    return SCPOperatorElStr3::Parse();
}

sc_result SCPOperatorStringSplit::Execute()
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

        string input(str1);
        vector<string> output;
        size_t current, previous = 0;
            current = str1.find_first_of(str2);
            while (current != std::string::npos) {
                output.push_back(str1.substr(previous, current - previous));
                previous = current + 1;
                current = str1.find_first_of(str2, previous);
            }
            output.push_back(str1.substr(previous, current - previous));
        ScAddr setNode = ms_context->CreateNode(ScType::NodeConst);
        for (int i = 0; i < output.size(); i++)
        {
            ScStreamPtr streamPtr = Utils::StreamFromString(output[i]);
            ScAddr answerLink = ms_context->CreateLink();

            ms_context->SetLinkContent(answerLink, *streamPtr);

            ms_context->CreateArc(sc_type_arc_pos_const_perm, setNode, answerLink);
            cout << output[i] << endl;
        }

            Utils::printOperatorAnswer(ms_context, operands[0], setNode);

            FinishExecutionSuccessfully();
    return SC_RESULT_OK;
}

}
