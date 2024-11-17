/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_type.hpp>

namespace scp
{
class SCPType : public ScType
{
public:
  using ScType::operator|;
  SCPType(ScType const & other);
};

}  // namespace scp
