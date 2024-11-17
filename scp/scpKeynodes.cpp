/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "scpKeynodes.hpp"

namespace scp
{

std::unordered_map<ScAddr, ScKeynode, ScAddrHashFunc> Keynodes::m_deprecatedKeynodes;
std::unordered_map<ScAddr, std::string, ScAddrHashFunc> Keynodes::m_deprecatedKeynodeIdentifiers;

void Keynodes::InitializeDeprecatedKeynodes()
{
  m_deprecatedKeynodes = {
      {Keynodes::rrel_link, Keynodes::rrel_node_link},
      {Keynodes::rrel_edge, Keynodes::rrel_common_edge},
      {Keynodes::rrel_common, Keynodes::rrel_common_arc},
      {Keynodes::rrel_access, Keynodes::rrel_membership_arc},
      {Keynodes::rrel_pos, Keynodes::rrel_pos_arc},
      {Keynodes::rrel_neg, Keynodes::rrel_neg_arc},
      {Keynodes::rrel_fuz, Keynodes::rrel_fuz_arc},
      {Keynodes::rrel_perm, Keynodes::rrel_perm_arc},
      {Keynodes::rrel_temp, Keynodes::rrel_temp_arc},
      {Keynodes::rrel_pos_const_perm, Keynodes::rrel_const_perm_pos_arc},
  };

  m_deprecatedKeynodeIdentifiers = {
      {Keynodes::rrel_link, Keynodes::rrel_link},
      {Keynodes::rrel_edge, Keynodes::rrel_edge},
      {Keynodes::rrel_common, Keynodes::rrel_common},
      {Keynodes::rrel_access, Keynodes::rrel_access},
      {Keynodes::rrel_pos, Keynodes::rrel_pos},
      {Keynodes::rrel_neg, Keynodes::rrel_neg},
      {Keynodes::rrel_fuz, Keynodes::rrel_fuz},
      {Keynodes::rrel_perm, Keynodes::rrel_perm},
      {Keynodes::rrel_temp, Keynodes::rrel_temp},
      {Keynodes::rrel_pos_const_perm, Keynodes::rrel_pos_const_perm},
  };
}

}  // namespace scp
