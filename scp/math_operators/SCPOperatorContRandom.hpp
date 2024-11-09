/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "scpOperatorStr.hpp"

namespace scp
{
class SCPOperatorContRandom : public SCPOperatorElStr3
{
public:
  SCPOperatorContRandom(ScAgentContext & ctx, ScAddr addr);
  std::string GetTypeName() override;
  sc_result Parse() override;
  sc_result Execute() override;

private:
  template <typename T>
  static T randomReal(T lowerBound, T upperBound);
  template <typename T>
  static T randomInt(T lowerBound, T upperBound);
};

}  // namespace scp
