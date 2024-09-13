#pragma once

#include <sc-memory/sc_agent.hpp>

namespace scp
{
class SCPInterpretationRequestInitiationAgent : public ScElementaryEventAgent
{
public:
  ScResult DoProgram(ScElementaryEvent const & event, ScAction & action) override;

  ScAddr GetAgentProgram() const;

  bool CheckInitiationCondition(ScElementaryEvent const & event) override;

  static int64_t GetTimeFromStart(std::chrono::time_point<std::chrono::high_resolution_clock> const & startTime);
};

}  // namespace scp
