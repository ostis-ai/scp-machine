/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "SCPOperatorIfEq.hpp"
#include "sc-memory/sc-memory/sc_memory.hpp"
#include "sc-memory/sc-memory/sc_stream.hpp"
#include <iostream>

namespace scp
{

SCPOperatorIfEq::SCPOperatorIfEq(const std::unique_ptr<ScMemoryContext> &ctx, ScAddr addr): SCPOperatorElStr2(ctx, addr)
{
}

std::string SCPOperatorIfEq::GetTypeName()
{
    return "ifEq";
}

sc_result SCPOperatorIfEq::Parse()
{
    return SCPOperatorElStr2::Parse();
}

sc_result SCPOperatorIfEq::Execute()
{
    std::cout << "SCPOperatorIfEq execute(): start"<<"\n";
    if (SC_RESULT_OK != ResetValues())
        return SC_RESULT_ERROR;

    if (!(operands[0]->IsFixed() && operands[1]->IsFixed()))
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(ms_context, "Both operands must have FIXED modifier", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }
    if (!operands[0]->GetValue().IsValid())
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(ms_context, "Operand 1 has modifier FIXED, but has no value", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    if (!operands[1]->GetValue().IsValid())
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(ms_context, "Operand 2 has modifier FIXED, but has no value", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    if (Utils::scLinkContentIsInt(ms_context, operands[0]->GetValue()) && Utils::scLinkContentIsInt(ms_context, operands[1]->GetValue()))
    {
        std::cout << "Links are int" << std::endl;
        if (Utils::scLinkGetContentInt(ms_context, operands[0]->GetValue()) == Utils::scLinkGetContentInt(ms_context, operands[1]->GetValue()))
        {
            FinishExecutionSuccessfully();
        }
        else
        {
            FinishExecutionUnsuccessfully();
        }
    }

    else if (Utils::scLinkContentIsUint(ms_context, operands[0]->GetValue()) && Utils::scLinkContentIsUint(ms_context, operands[1]->GetValue()))
    {
        std::cout << "Links are uint" << std::endl;
        if (Utils::scLinkGetContentUint(ms_context, operands[0]->GetValue()) == Utils::scLinkGetContentUint(ms_context, operands[1]->GetValue()))
        {
            FinishExecutionSuccessfully();
        }
        else
        {
            FinishExecutionUnsuccessfully();
        }
    }

    else if (Utils::scLinkContentIsFloat(ms_context, operands[0]->GetValue()) && Utils::scLinkContentIsFloat(ms_context, operands[1]->GetValue()))
    {
        std::cout << "Links are float" << std::endl;
        if (Utils::scLinkGetContentFloat(ms_context, operands[0]->GetValue()) == Utils::scLinkGetContentFloat(ms_context, operands[1]->GetValue()))
        {
            FinishExecutionSuccessfully();
        }
        else
        {
            FinishExecutionUnsuccessfully();
        }
    }

    else if (Utils::scLinkContentIsDouble(ms_context, operands[0]->GetValue()) && Utils::scLinkContentIsDouble(ms_context, operands[1]->GetValue()))
    {
        std::cout << "Links are double" << std::endl;
        if (Utils::scLinkGetContentDouble(ms_context, operands[0]->GetValue()) == Utils::scLinkGetContentDouble(ms_context, operands[1]->GetValue()))
        {
            FinishExecutionSuccessfully();
        }
        else
        {
            FinishExecutionUnsuccessfully();
        }
    }

    else {
        if(!Utils::scLinkPlainNumbers(ms_context, operands[0]->GetValue()).empty() && !Utils::scLinkPlainNumbers(ms_context, operands[1]->GetValue()).empty()){
            std::string str1 = Utils::scLinkPlainNumbers(ms_context, operands[0]->GetValue());
            std::string str2 = Utils::scLinkPlainNumbers(ms_context, operands[1]->GetValue());
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
                if (num1 == num2){
                    FinishExecutionSuccessfully();
                }
                else {
                    FinishExecutionUnsuccessfully();
                }
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
                if (num1 == num2){
                    FinishExecutionSuccessfully();
                }
                else {
                    FinishExecutionUnsuccessfully();
                }
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

    return SC_RESULT_OK;
}

}
