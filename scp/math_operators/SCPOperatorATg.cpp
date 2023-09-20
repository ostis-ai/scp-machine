/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/


#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "SCPOperatorATg.hpp"
#include "sc-core/sc_helper.h"
#include "sc-core/sc_memory_headers.h"
#include "sc-memory/sc-memory/sc_stream.hpp"
#include <iostream>
#include <math.h>

namespace scp
{

SCPOperatorATg::SCPOperatorATg(const std::unique_ptr<ScMemoryContext> &ctx, ScAddr addr): SCPOperatorElStr2(ctx, addr)
{
}

std::string SCPOperatorATg::GetTypeName()
{
    return "contATg";
}

sc_result SCPOperatorATg::Parse()
{
    return SCPOperatorElStr2::Parse();
}

sc_result SCPOperatorATg::Execute()
{
    std::cout << "SCPOperatorSIn execute(): start";
    if (SC_RESULT_OK != ResetValues())
        return SC_RESULT_ERROR;

    if (!(operands[1]->IsFixed()))
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(ms_context, "Second operand must have FIXED modifier", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    if (!operands[1]->GetValue().IsValid())
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(ms_context, "Second operand is FIXED, but has no value", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    std::string answer_str;

    if (Utils::scLinkContentIsInt(ms_context, operands[1]->GetValue()) == true)
    {
        std::cout << "Link is INT" << std::endl;
        int value = Utils::scLinkGetContentInt(ms_context, operands[1]->GetValue());
        int answer = (int)atan((double)value);
        answer_str = std::to_string(answer);
    }

    if (Utils::scLinkContentIsUint(ms_context, operands[1]->GetValue()) == true)
    {
        std::cout << "Link is UINT" << std::endl;
        int value = Utils::scLinkGetContentUint(ms_context, operands[1]->GetValue());
        int answer = (int)atan((double)value);
        answer_str = std::to_string(answer);
    }

    if (Utils::scLinkContentIsFloat(ms_context, operands[1]->GetValue()) == true)
    {
        std::cout << "Link is FLOAT" << std::endl;
        float value = Utils::scLinkGetContentFloat(ms_context, operands[1]->GetValue());
        float answer = (float)atan((double)value);
        answer_str = std::to_string(answer);
    }

    if (Utils::scLinkContentIsDouble(ms_context, operands[1]->GetValue()) == true)
    {
        std::cout << "Link is DOUBLE" << std::endl;
        double value = Utils::scLinkGetContentDouble(ms_context, operands[1]->GetValue());
        double answer = atan(value);
        answer_str = std::to_string(answer);
    }

    if (answer_str.empty()){
        if(!Utils::scLinkPlainNumbers(ms_context, operands[1]->GetValue()).empty()){
            answer_str = Utils::scLinkPlainNumbers(ms_context, operands[1]->GetValue());
            std::string intInit = "int: ";
            std::string::size_type i = answer_str.find(intInit);

            if (i != std::string::npos)
            {
                std::cout << "Link is INT" << std::endl;
                answer_str.erase(i, intInit.length());
                int value = std::atoi(answer_str.c_str());
                int answer = (int)atan((double)value);
                answer_str = std::to_string(answer);
            }

            std::string intDouble = "double: ";
            std::string::size_type d = answer_str.find(intDouble);

            if (d != std::string::npos)
            {
                std::cout << "Link is Double" << std::endl;
                answer_str.erase(d, intDouble.length());
                double value = std::stod(answer_str);
                double answer = atan(value);
                answer_str = std::to_string(answer);
            }
        }
        else{
            FinishExecutionUnsuccessfully();
            #ifdef SCP_DEBUG
                Utils::logSCPError(ms_context, "Second operand is not numeric!", addr);
            #endif
            return SC_RESULT_OK;
        }
    }

    std::cout << "Link: " << answer_str << std::endl;

    ScAddr answer_link = ms_context->CreateLink();
    ScStreamPtr streamPtr = Utils::StreamFromString(answer_str);

    ScAddr elem1, elem3, elem5, arc1;
    elem1.Reset();
    elem3.Reset();
    elem5.Reset();
    arc1.Reset();

    elem5 = Keynodes::nrel_scp_var_value;
    ms_context->SetLinkContent(answer_link, streamPtr);
    elem3 = answer_link;
    elem1 = operands[0]->CreateNodeOrLink();

    arc1 = ms_context->CreateEdge(sc_type_arc_common, elem1, elem3);
    ms_context->CreateEdge(sc_type_arc_pos_const_perm, elem5, arc1);
    Utils::printInfo(ms_context, elem5);
    Utils::printInfo(ms_context, elem3);
    Utils::printInfo(ms_context, elem1);
    std::cout << ms_context->IsElement(elem1);
    operands[0]->SetValue(elem1);

    std::cout << "SCPOperatorSIn execute(): end";
    FinishExecutionSuccessfully();
    return SC_RESULT_OK;
}

}
