/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>

namespace scp
{
class ASCPHandlingEventThatInitiatesAgentSCPProgram : public ScElementaryEventAgent
{
public:
  ScResult DoProgram(ScElementaryEvent const & event, ScAction & action) override;

  ScAddr GetAgentProgram() const;
};

}  // namespace scp
