/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"
#include <sc-memory/sc_agent.hpp>
#include "scpKeynodes.hpp"

namespace scp
{

class ASCPProcessCreator : public ScActionInitiatedAgent
{
    public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

    public:
    static inline ScKeynode const msAgentKeynode{"sc_agent_of_scp_process_creation"};

};

}

