#pragma once

#include "sc-memory/sc-memory/sc_addr.hpp"
#include "sc-memory/sc-memory/sc_object.hpp"
#include "sc-memory/sc-memory/kpm/sc_agent.hpp"
#include "scpKeynodes.hpp"
#include "scpOperatorStr.hpp"
namespace scp
{

class SCPOperatorATg: public SCPOperatorElStr2
{
public:
    SCPOperatorATg(const std::unique_ptr<ScMemoryContext> &ctx, ScAddr addr);
    std::string GetTypeName();
    sc_result Parse();
    sc_result Execute();
};
}
