#include "scpmathoperatorinterpreter.hpp"

#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "math_operators/scpoperatorsin.hpp"
#include "math_operators/scpoperatorcos.hpp"
#include "math_operators/scpoperatortg.hpp"
#include "math_operators/scpoperatorasin.hpp"
#include "math_operators/scpoperatoracos.hpp"
#include "math_operators/scpoperatoratg.hpp"
#include "math_operators/scpoperatordivint.hpp"
#include "math_operators/scpoperatordivrem.hpp"
#include "math_operators/SCPOperatorIfEq.hpp"
#include "math_operators/SCPOperatorIfGr.hpp"
#include "math_operators/SCPOperatorContLn.hpp"
#include "math_operators/SCPOperatorContAdd.hpp"
#include "math_operators/SCPOperatorContSub.hpp"
#include "math_operators/SCPOperatorContMult.hpp"
#include "math_operators/SCPOperatorContPow.hpp"
#include "math_operators/SCPOperatorContDiv.hpp"

#include "sc-memory/cpp/sc_memory.hpp"
#include <iostream>

namespace scp {
ScAddr SCPMathOperatorInterpreter::msAgentKeynode;

SC_AGENT_IMPLEMENTATION(SCPMathOperatorInterpreter){
    if (!edgeAddr.IsValid())
        return SC_RESULT_ERROR;

    ScAddr scp_operator = ms_context->GetArcEnd(edgeAddr);

    ScAddr type;
    if (SC_TRUE != Utils::resolveOperatorType(ms_context, scp_operator, type))
        return SC_RESULT_ERROR_INVALID_TYPE;

    SCPOperator* oper = nullptr;
    std::cout << "SCPMathOperatorInterpreter: start \n";
    std::cout << "Hello!!!\n";

    if (type == Keynodes::op_contSin)
    {
        std::cout << "SCPMathOperatorInterpreter: SCPOperatorSIn is chosen \n";
        oper = new SCPOperatorSIn(ms_context, scp_operator);
    }
    if (type == Keynodes::op_contCos)
    {
        std::cout << "SCPMathOperatorInterpreter: SCPOperatorCos is chosen \n";
        oper = new SCPOperatorCos(ms_context, scp_operator);
    }
    if (type == Keynodes::op_contTg)
    {
        oper = new SCPOperatorTg(ms_context, scp_operator);
    }
    if (type == Keynodes::op_contASin)
    {
        oper = new SCPOperatorASin(ms_context, scp_operator);
    }
    if (type == Keynodes::op_contACos)
    {
        oper = new SCPOperatorACos(ms_context, scp_operator);
    }
    if (type == Keynodes::op_contATg)
    {
        oper = new SCPOperatorATg(ms_context, scp_operator);
    }
    if (type == Keynodes::op_contDivInt)
    {
        std::cout << "SCPMathOperatorInterpreter: SCPOperatorDivInt is chosen \n";
        oper = new SCPOperatorDivInt(ms_context, scp_operator);
    }
    if (type == Keynodes::op_contDivRem)
    {
        oper = new SCPOperatorDivRem(ms_context, scp_operator);
    }
    if (type == Keynodes::op_ifEq)
    {
        oper = new SCPOperatorIfEq(ms_context, scp_operator);
    }
    if (type == Keynodes::op_contLn)
    {
        oper = new SCPOperatorContLn(ms_context, scp_operator);
    }
    if (type == Keynodes::op_ifGr)
    {
        oper = new SCPOperatorIfGr(ms_context, scp_operator);
    }
    if (type == Keynodes::op_contAdd)
    {
        oper = new SCPOperatorContAdd(ms_context, scp_operator);
    }
    if (type == Keynodes::op_contSub)
    {
        oper = new SCPOperatorContSub(ms_context, scp_operator);
    }
    if (type == Keynodes::op_contMult)
    {
        oper = new SCPOperatorContMult(ms_context, scp_operator);
    }
    if (type == Keynodes::op_contDiv)
    {
        oper = new SCPOperatorContDiv(ms_context, scp_operator);
    }
    if (type == Keynodes::op_contPow)
    {
        oper = new SCPOperatorContPow(ms_context, scp_operator);
    }
    if (oper == nullptr){
        std::cout << "Null \n";
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }
#ifdef SCP_DEBUG
    std::cout << oper->GetTypeName() << std::endl;
#endif

    sc_result parse_result = oper->Parse();
    if (parse_result != SC_RESULT_OK)
    {
        delete oper;
        std::cout << "SCPMathOperatorInterpreter: end with ex \n";
        return parse_result;
    }
    else
    {
        std::cout << "SCPMathOperatorInterpreter: end\n";
        sc_result execute_result;
        execute_result = oper->Execute();
        delete oper;
        return execute_result;
    }
}

}
