/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-memory/sc_addr.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "scpOperand.hpp"

#include "scp.hpp"

#include <iostream>

namespace scp
{

SCPOperand::SCPOperand(ScAgentContext & ctx_, ScAddr addr_)
  : arc_addr(addr_)
  , m_memoryCtx(ctx_)
{
  addr = m_memoryCtx.GetArcTargetElement(arc_addr);
  resolveModifiers();
  value_addr.Reset();
  if (operand_type == SCP_CONST)
  {
    value_addr = addr;
  }
  else
  {
    ScIterator3Ptr iter = m_memoryCtx.CreateIterator3(addr, ScType::ConstTempPosArc, ScType::Unknown);
    if (iter->Next())
    {
      value_addr = iter->Get(2);
    }
  }
}

ScAddr SCPOperand::GetAddr()
{
  return addr;
}

ScAddr SCPOperand::GetValue()
{
  return value_addr;
}

ScType SCPOperand::GetType()
{
  return element_type;
}

uint8_t SCPOperand::GetOrder()
{
  return order;
}

uint8_t SCPOperand::GetSetOrder()
{
  return set_order;
}

void SCPOperand::ResetValue()
{
  ScIterator3Ptr iter = m_memoryCtx.CreateIterator3(addr, ScType::ConstTempPosArc, ScType::Unknown);
  while (iter->Next())
  {
    m_memoryCtx.EraseElement(iter->Get(1));
  }
  value_addr.Reset();
}

void SCPOperand::SetValue(ScAddr value)
{
  value_addr = value;
  m_memoryCtx.GenerateConnector(ScType::ConstTempPosArc, addr, value);
}

ScAddr SCPOperand::CreateNodeOrLink()
{
  if (element_type.IsLink())
    value_addr = m_memoryCtx.GenerateLink();
  else
    value_addr = m_memoryCtx.GenerateNode(element_type);
  m_memoryCtx.GenerateConnector(ScType::ConstTempPosArc, addr, value_addr);
  return value_addr;
}

bool SCPOperand::IsFixed()
{
  return (param_type == SCP_FIXED);
}

bool SCPOperand::IsAssign()
{
  return (param_type == SCP_ASSIGN);
}

bool SCPOperand::IsSCPConst()
{
  return (operand_type == SCP_CONST);
}

bool SCPOperand::IsSCPVar()
{
  return (operand_type == SCP_VAR);
}

bool SCPOperand::IsErase()
{
  return isErase;
}

bool SCPOperand::IsSet()
{
  return isSet;
}

bool SCPOperand::HasSetModifier()
{
  return isSet;
}

bool SCPOperand::HasEraseModifier()
{
  return isErase;
}

void SCPOperand::resolveOrder(ScAddr modifier)
{
  if (modifier == Keynodes::rrel_1)
  {
    order = 1;
    return;
  }
  if (modifier == Keynodes::rrel_2)
  {
    order = 2;
    return;
  }
  if (modifier == Keynodes::rrel_3)
  {
    order = 3;
    return;
  }
  if (modifier == Keynodes::rrel_4)
  {
    order = 4;
    return;
  }
  if (modifier == Keynodes::rrel_5)
  {
    order = 5;
    return;
  }
  if (modifier == Keynodes::rrel_6)
  {
    order = 6;
    return;
  }
  if (modifier == Keynodes::rrel_7)
  {
    order = 7;
    return;
  }
  if (modifier == Keynodes::rrel_8)
  {
    order = 8;
    return;
  }
  if (modifier == Keynodes::rrel_9)
  {
    order = 9;
    return;
  }
  if (modifier == Keynodes::rrel_10)
  {
    order = 10;
    return;
  }
}

void SCPOperand::resolveSetOrder(ScAddr modifier)
{
  if (modifier == Keynodes::rrel_set_1)
  {
    set_order = 1;
    return;
  }
  if (modifier == Keynodes::rrel_set_2)
  {
    set_order = 2;
    return;
  }
  if (modifier == Keynodes::rrel_set_3)
  {
    set_order = 3;
    return;
  }
  if (modifier == Keynodes::rrel_set_4)
  {
    set_order = 4;
    return;
  }
  if (modifier == Keynodes::rrel_set_5)
  {
    set_order = 5;
    return;
  }
}

void SCPOperand::resolveModifiers()
{
  ScIterator3Ptr iter = m_memoryCtx.CreateIterator3(ScType::ConstNode, ScType::ConstPermPosArc, arc_addr);
  while (iter->Next())
  {
    ScAddr modifier = iter->Get(0);
    if (m_memoryCtx.CheckConnector(Keynodes::order_role_relation, modifier, ScType::ConstPermPosArc))
    {
      resolveOrder(modifier);
      continue;
    }

    auto const & deprecatedKeynodes = Keynodes::GetDeprecatedKeynodes();
    auto const & it = deprecatedKeynodes.find(modifier);
    if (it != deprecatedKeynodes.cend())
      SCP_LOG_WARNING(
          "Role relation `" << std::string(it->second.first) << "` is deprecated. Use role relation `"
                            << std::string(it->second.second) << "` instead.");

    if (order == 0 && set_order == 0)
    {
      resolveSetOrder(modifier);
      if (set_order > 0)
        continue;
    }

    if (modifier == Keynodes::rrel_scp_const)
    {
      operand_type = SCP_CONST;
      continue;
    }
    if (modifier == Keynodes::rrel_scp_var)
    {
      operand_type = SCP_VAR;
      continue;
    }

    if (modifier == Keynodes::rrel_fixed)
    {
      param_type = SCP_FIXED;
      continue;
    }
    if (modifier == Keynodes::rrel_assign)
    {
      param_type = SCP_ASSIGN;
      continue;
    }

    if (modifier == Keynodes::rrel_set)
    {
      isSet = SC_TRUE;
      continue;
    }
    if (modifier == Keynodes::rrel_erase)
    {
      isErase = SC_TRUE;
      continue;
    }

    if (modifier == Keynodes::rrel_const)
    {
      element_type = element_type | ScType::Const;
      continue;
    }
    if (modifier == Keynodes::rrel_var)
    {
      element_type = element_type | ScType::Var;
      continue;
    }
    if (modifier == Keynodes::rrel_node)
    {
      element_type = element_type | ScType::Node;
      continue;
    }
    if (modifier == Keynodes::rrel_node_link || modifier == Keynodes::rrel_link)
    {
      element_type = element_type | ScType::NodeLink;
      continue;
    }
    if (modifier == Keynodes::rrel_structure || modifier == Keynodes::rrel_struct)
    {
      element_type = element_type | ScType::NodeStructure;
      continue;
    }
    if (modifier == Keynodes::rrel_non_role_relation)
    {
      element_type = element_type | ScType::NodeNonRole;
      continue;
    }
    if (modifier == Keynodes::rrel_role_relation)
    {
      element_type = element_type | ScType::NodeRole;
      continue;
    }
    if (modifier == Keynodes::rrel_class)
    {
      element_type = element_type | ScType::NodeClass;
      continue;
    }

    if (modifier == Keynodes::rrel_connector)
    {
      element_type = element_type | ScType::Connector;
      continue;
    }
    if (modifier == Keynodes::rrel_common_edge || modifier == Keynodes::rrel_edge)
    {
      element_type = element_type | ScType::CommonEdge;
      continue;
    }
    if (modifier == Keynodes::rrel_arc)
    {
      element_type = element_type | ScType::Arc;
      continue;
    }
    if (modifier == Keynodes::rrel_common_arc || modifier == Keynodes::rrel_common)
    {
      element_type = element_type | ScType::CommonArc;
      continue;
    }
    if (modifier == Keynodes::rrel_membership_arc || modifier == Keynodes::rrel_access)
    {
      element_type = element_type | ScType::MembershipArc;
      continue;
    }

    if (modifier == Keynodes::rrel_temp_arc || modifier == Keynodes::rrel_temp)
    {
      element_type = element_type | ScType::TempArc;
      continue;
    }
    if (modifier == Keynodes::rrel_actual_arc)
    {
      element_type = element_type | ScType::ActualTempArc;
      continue;
    }
    if (modifier == Keynodes::rrel_inactual_arc)
    {
      element_type = element_type | ScType::InactualTempArc;
      continue;
    }
    if (modifier == Keynodes::rrel_perm_arc || modifier == Keynodes::rrel_perm)
    {
      element_type = element_type | ScType::PermArc;
      continue;
    }
    if (modifier == Keynodes::rrel_pos_arc || modifier == Keynodes::rrel_pos)
    {
      element_type = element_type | ScType::PosArc;
      continue;
    }
    if (modifier == Keynodes::rrel_neg_arc || modifier == Keynodes::rrel_neg)
    {
      element_type = element_type | ScType::NegArc;
      continue;
    }
    if (modifier == Keynodes::rrel_fuz_arc || modifier == Keynodes::rrel_fuz)
    {
      element_type = element_type | ScType::FuzArc;
      continue;
    }
    if (modifier == Keynodes::rrel_const_perm_pos_arc || modifier == Keynodes::rrel_pos_const_perm)
    {
      element_type = element_type | ScType::ConstPermPosArc;
      continue;
    }
  }
}

}  // namespace scp
