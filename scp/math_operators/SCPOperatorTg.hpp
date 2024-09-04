#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"
#include <sc-memory/sc_agent.hpp>
#include "scpKeynodes.hpp"
#include "scpOperatorStr.hpp"
namespace scp
{

class SCPOperatorTg: public SCPOperatorElStr2
{
public:
    SCPOperatorTg(ScMemoryContext &ctx, ScAddr addr);
    std::string GetTypeName();
    sc_result Parse();
    sc_result Execute();
};
}
