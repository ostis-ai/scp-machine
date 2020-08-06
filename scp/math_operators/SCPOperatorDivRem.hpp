#pragma once

#include "sc-memory/cpp/sc_addr.hpp"
#include "sc-memory/cpp/sc_object.hpp"
#include "sc-memory/cpp/kpm/sc_agent.hpp"
#include "scpKeynodes.hpp"
#include "scpOperatorStr.hpp"
namespace scp
{

class SCPOperatorDivRem: public SCPOperatorElStr3
{
public:
    SCPOperatorDivRem(const std::unique_ptr<ScMemoryContext> &ctx, ScAddr addr);
    std::string GetTypeName();
    sc_result Parse();
    sc_result Execute();
};
}
