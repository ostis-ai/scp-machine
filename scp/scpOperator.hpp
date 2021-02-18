/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/cpp/sc_addr.hpp"
#include "scpKeynodes.hpp"
#include "scpOperand.hpp"

#include <vector>

namespace scp {

class SCPOperator
{
protected:
    ScAddr addr;
    ScAddr type;
    std::vector<SCPOperand*> operands;

    const std::unique_ptr<ScMemoryContext>& ms_context;

    sc_result ResetValues();
    sc_result CheckNullValues();

public:
    SCPOperator(const std::unique_ptr<ScMemoryContext>& ctx_, ScAddr addr_);
    virtual ~SCPOperator();
    ScAddr GetAddr();
    virtual std::string GetTypeName();
    virtual sc_result Parse();
    virtual sc_result Execute();
    void ClearExecutionState();
    void FinishExecution();
    void FinishExecutionSuccessfully();
    void FinishExecutionUnsuccessfully();
    void FinishExecutionWithError();
    static void ClearExecutionState(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr oper_addr);
    static void FinishExecution(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr oper_addr);
    static void FinishExecutionSuccessfully(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr oper_addr);
    static void FinishExecutionUnsuccessfully(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr oper_addr);
    static void FinishExecutionWithError(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr oper_addr);
};

}

