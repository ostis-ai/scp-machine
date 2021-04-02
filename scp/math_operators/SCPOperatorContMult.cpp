/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "SCPOperatorContMult.hpp"
#include "sc-memory/sc-memory/sc_memory.hpp"
#include "sc-memory/sc-memory/sc_stream.hpp"
#include <iostream>

namespace scp
{

SCPOperatorContMult::SCPOperatorContMult(const std::unique_ptr<ScMemoryContext> &ctx, ScAddr addr): SCPOperatorElStr3(ctx, addr)
{
}

std::string SCPOperatorContMult::GetTypeName()
{
    return "contMult";
}

sc_result SCPOperatorContMult::Parse()
{
    return SCPOperatorElStr3::Parse();
}

sc_result SCPOperatorContMult::Execute()
{
    std::cout << "SCPOperatorContMult execute(): start"<<"\n";
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

    std::string answer_str;

    if (Utils::scLinkContentIsInt(ms_context, operands[1]->GetValue()) && Utils::scLinkContentIsInt(ms_context, operands[2]->GetValue()))
    {
        std::cout << "Links are int" << std::endl;
        int number1 = Utils::scLinkGetContentInt(ms_context, operands[1]->GetValue());
        int number2 = Utils::scLinkGetContentInt(ms_context, operands[2]->GetValue());
        int answer = number1 * number2;
        answer_str = std::to_string(answer);
    }

    if (Utils::scLinkContentIsUint(ms_context, operands[1]->GetValue()) && Utils::scLinkContentIsUint(ms_context, operands[2]->GetValue()))
    {
        std::cout << "Links are uint" << std::endl;
        int number1 = Utils::scLinkGetContentUint(ms_context, operands[1]->GetValue());
        int number2 = Utils::scLinkGetContentUint(ms_context, operands[2]->GetValue());
        int answer = number1 * number2;
        answer_str = std::to_string(answer);
    }

    if (Utils::scLinkContentIsFloat(ms_context, operands[1]->GetValue()) && Utils::scLinkContentIsFloat(ms_context, operands[2]->GetValue()))
    {
        std::cout << "Links are float" << std::endl;
        float number1 = Utils::scLinkGetContentFloat(ms_context, operands[1]->GetValue());
        float number2 = Utils::scLinkGetContentFloat(ms_context, operands[2]->GetValue());
        float answer = (float)(number1 * number2);
        answer_str = std::to_string(answer);
    }

    if (Utils::scLinkContentIsDouble(ms_context, operands[1]->GetValue()) && Utils::scLinkContentIsDouble(ms_context, operands[2]->GetValue()))
    {
        std::cout << "Links are double" << std::endl;
        double number1 = Utils::scLinkGetContentDouble(ms_context, operands[1]->GetValue());
        double number2 = Utils::scLinkGetContentDouble(ms_context, operands[2]->GetValue());
        double answer = (double)(number1 * number2);
        answer_str = std::to_string(answer);

    }

    if (answer_str.empty()){
        if(!Utils::scLinkPlainNumbers(ms_context, operands[1]->GetValue()).empty() && !Utils::scLinkPlainNumbers(ms_context, operands[2]->GetValue()).empty()){
            std::string str1 = Utils::scLinkPlainNumbers(ms_context, operands[1]->GetValue());
            std::string str2 = Utils::scLinkPlainNumbers(ms_context, operands[2]->GetValue());
            std::string intInit = "int: ";
            std::string::size_type i1 = str1.find(intInit);
            std::string::size_type i2 = str2.find(intInit);

            if (i1 != std::string::npos && i2 != std::string::npos)
            {
                std::cout << "Link is INT" << std::endl;
                str1.erase(i1, intInit.length());
                str2.erase(i2, intInit.length());
                int num1 = std::atoi(str1.c_str());
                int num2 = std::atoi(str2.c_str());
                int answer = num1 * num2;
                answer_str = std::to_string(answer);
            }

            std::string intDouble = "double: ";
            std::string::size_type d1 = str1.find(intDouble);
            std::string::size_type d2 = str2.find(intDouble);

            if ((d1 != std::string::npos) && (d2 != std::string::npos))
            {
                std::cout << "Link is Double" << std::endl;
                str1.erase(d1, intDouble.length());
                str2.erase(d2, intDouble.length());
                double num1 = std::stod(str1);
                double num2 = std::stod(str2);
                double answer = num1 * num2;
                answer_str = std::to_string(answer);
            }
        }
        else{
            FinishExecutionUnsuccessfully();
            #ifdef SCP_DEBUG
                Utils::logSCPError(ms_context, "Both operands are not numeric!", addr);
            #endif
            return SC_RESULT_OK;
        }
    }

    ScStreamPtr streamPtr = Utils::StreamFromString(answer_str);
    ScAddr answerLink = ms_context->CreateLink();
    ms_context->SetLinkContent(answerLink, streamPtr);

    ScAddr elem1, elem3, elem5, arc1;
    elem1.Reset();
    elem3.Reset();
    elem5.Reset();
    arc1.Reset();

    elem1 = operands[0]->GetValue();
    elem5 = Keynodes::nrel_scp_var_value;
    elem3 = answerLink;
    elem1 = operands[0]->CreateNodeOrLink();
    arc1 = ms_context->CreateArc(sc_type_arc_common, elem1, elem3);
    ms_context->CreateArc(sc_type_arc_pos_const_perm, elem5, arc1);
    Utils::printInfo(ms_context, elem5);
    Utils::printInfo(ms_context, elem3);
    operands[0]->SetValue(elem1);

    std::cout << "SCPOperatorContMult execute(): end";
    FinishExecutionSuccessfully();
    return SC_RESULT_OK;
}

}


