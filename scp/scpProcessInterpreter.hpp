/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>

namespace scp
{

class ASCPProcessInterpreter : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScAction & action) override;

  bool CheckInitiationCondition(ScActionInitiatedEvent const & event) override;

  static inline ScKeynode const msAgentKeynode{"sc_agent_of_scp_process_interpretation"};
};

}  // namespace scp