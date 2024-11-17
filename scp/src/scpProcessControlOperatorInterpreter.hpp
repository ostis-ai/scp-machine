/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "scpOperatorInterpreter.hpp"

namespace scp
{

class ASCPProcessControlOperatorInterpreter : public ASCPOperatorInterpreter
{
public:
  ScAddr GetActionClass() const override;

  ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
  getSupportedOperators() const override;

  static inline ScKeynode const msAgentKeynode{"sc_agent_of_scp_operator_scp_process_control_interpretation"};

private:
  static ScAddrToValueUnorderedMap<std::function<std::unique_ptr<SCPOperator>(ScAgentContext &, ScAddr)>>
      supportedOperators;
};

}  // namespace scp
