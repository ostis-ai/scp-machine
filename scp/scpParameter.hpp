/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "scpKeynodes.hpp"

namespace scp
{

class SCPParameter
{
private:
  ScAddr addr;
  ScAddr arc_addr;
  bool isIn = false;
  bool isOut = false;
  uint8_t order = 0;
  ScMemoryContext & m_memoryCtx;

  void resolveModifiers();
  void resolveOrder(ScAddr modifier);

public:
  SCPParameter(ScMemoryContext & ctx_, ScAddr addr_);
  ScAddr GetAddr();
  uint8_t GetOrder();
  bool IsIn();
  bool IsOut();
};

}  // namespace scp
