/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/


#include "scpKeynodes.hpp"
#include "scpUtils.hpp"
#include "SCPOperatorDivInt.hpp"
#include "sc-core/sc_helper.h"
#include "sc-core/sc_memory_headers.h"
#include "sc-memory/sc_stream.hpp"
#include <iostream>
#include <math.h>

namespace scp
{

SCPOperatorDivInt::SCPOperatorDivInt(ScMemoryContext &ctx, ScAddr addr): SCPOperatorElStr3(ctx, addr)
{
}

std::string SCPOperatorDivInt::GetTypeName()
{
    return "contDivInt";
}

sc_result SCPOperatorDivInt::Parse()
{
    return SCPOperatorElStr3::Parse();
}

sc_result SCPOperatorDivInt::Execute()
{
    std::cout << "SCPOperatorDivInt execute(): start";
    if (SC_RESULT_OK != ResetValues())
        return SC_RESULT_ERROR;

    if (!(operands[1]->IsFixed()))
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(m_memoryCtx, "Second operand must have FIXED modifier", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    if (!operands[1]->GetValue().IsValid())
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(m_memoryCtx, "Second operand is FIXED, but has no value", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    if (!(operands[2]->IsFixed()))
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(m_memoryCtx, "Third operand must have FIXED modifier", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    if (!operands[2]->GetValue().IsValid())
    {
#ifdef SCP_DEBUG
        Utils::logSCPError(m_memoryCtx, "Third operand is FIXED, but has no value", addr);
#endif
        FinishExecutionWithError();
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    int firstOperand;
    int secondOperand;

    if (Utils::scLinkContentIsInt(m_memoryCtx, operands[1]->GetValue()) == true)
    {
        std::cout << "Link is INT" << std::endl;
        firstOperand = Utils::scLinkGetContentInt(m_memoryCtx, operands[1]->GetValue());
    }

    if (Utils::scLinkContentIsUint(m_memoryCtx, operands[1]->GetValue()) == true)
    {
        std::cout << "Link is UINT" << std::endl;
        firstOperand = Utils::scLinkGetContentUint(m_memoryCtx, operands[1]->GetValue());
    }

    if (Utils::scLinkContentIsFloat(m_memoryCtx, operands[1]->GetValue()) == true)
    {
        std::cout << "Link is FLOAT" << std::endl;
        firstOperand = (int)Utils::scLinkGetContentFloat(m_memoryCtx, operands[1]->GetValue());
    }

    if (Utils::scLinkContentIsDouble(m_memoryCtx, operands[1]->GetValue()) == true)
    {
        std::cout << "Link is DOUBLE" << std::endl;
        firstOperand = (int)Utils::scLinkGetContentDouble(m_memoryCtx, operands[1]->GetValue());
    }

    if (Utils::scLinkContentIsInt(m_memoryCtx, operands[2]->GetValue()) == true)
    {
        std::cout << "Link is INT" << std::endl;
        secondOperand = Utils::scLinkGetContentInt(m_memoryCtx, operands[2]->GetValue());
    }

    if (Utils::scLinkContentIsUint(m_memoryCtx, operands[2]->GetValue()) == true)
    {
        std::cout << "Link is UINT" << std::endl;
        secondOperand = Utils::scLinkGetContentUint(m_memoryCtx, operands[2]->GetValue());
    }

    if (Utils::scLinkContentIsFloat(m_memoryCtx, operands[2]->GetValue()) == true)
    {
        std::cout << "Link is FLOAT" << std::endl;
        secondOperand = (int)Utils::scLinkGetContentFloat(m_memoryCtx, operands[2]->GetValue());
    }

    if (Utils::scLinkContentIsDouble(m_memoryCtx, operands[2]->GetValue()) == true)
    {
        std::cout << "Link is DOUBLE" << std::endl;
        secondOperand = (int)Utils::scLinkGetContentDouble(m_memoryCtx, operands[2]->GetValue());
    }

    int ans =  div(firstOperand, secondOperand).quot;

    std::string answer_str = std::to_string(ans);

    std::cout << "Link: " << answer_str << std::endl;

    ScAddr answer_link =m_memoryCtx.CreateLink();
    ScStreamPtr streamPtr = Utils::StreamFromString(answer_str);

    ScAddr elem1, elem3, elem5, arc1;
    elem1.Reset();
    elem3.Reset();
    elem5.Reset();
    arc1.Reset();

    elem5 = Keynodes::nrel_scp_var_value;
   m_memoryCtx.SetLinkContent(answer_link, streamPtr);
    elem3 = answer_link;
    elem1 = operands[0]->CreateNodeOrLink();

    arc1 =m_memoryCtx.CreateEdge(sc_type_arc_common, elem1, elem3);
   m_memoryCtx.CreateEdge(sc_type_arc_pos_const_perm, elem5, arc1);
    Utils::printInfo(m_memoryCtx, elem5);
    Utils::printInfo(m_memoryCtx, elem3);
    Utils::printInfo(m_memoryCtx, elem1);
    std::cout <<m_memoryCtx.IsElement(elem1);
    operands[0]->SetValue(elem1);

    //int answer = div(firstOperand, secondOperand);
    FinishExecutionSuccessfully();
    return SC_RESULT_OK;
}

}
