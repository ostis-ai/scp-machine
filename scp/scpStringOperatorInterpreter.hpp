#pragma once

#include "sc-memory/cpp/sc_addr.hpp"
#include "sc-memory/cpp/sc_object.hpp"
#include "sc-memory/cpp/kpm/sc_agent.hpp"
#include "scpKeynodes.hpp"

#include "scpStringOperatorInterpreter.generated.hpp"

namespace scp
{

class ASCPStringOperatorInterpreter : public ScAgent
{
    SC_CLASS(Agent, Event(Keynodes::active_action, ScEvent::Type::AddOutputEdge))
    SC_GENERATED_BODY()

public:
    SC_PROPERTY(Keynode("sc_agent_of_scp_operator_string_interpreting"), ForceCreate)
    static ScAddr msAgentKeynode;

};

}

