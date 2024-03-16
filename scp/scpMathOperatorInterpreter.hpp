#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"
#include "sc-memory/kpm/sc_agent.hpp"
#include "scpKeynodes.hpp"

#include "generated/scpMathOperatorInterpreter.generated.hpp"

namespace scp
{

class ASCPMathOperatorInterpreter : public ScAgent
{
    SC_CLASS(Agent, Event(Keynodes::active_action, ScEvent::Type::AddOutputEdge))
    SC_GENERATED_BODY()

public:
    SC_PROPERTY(Keynode("sc_agent_of_scp_operator_math_interpreting"), ForceCreate)
    static ScAddr msAgentKeynode;

};

}

