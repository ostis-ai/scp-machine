/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "sc-memory/sc-memory/sc_addr.hpp"
#include "scpKeynodes.hpp"
#include "scpUtils.hpp"

#include "scpOperand.hpp"

#include <iostream>

namespace scp {

SCPOperand::SCPOperand(const std::unique_ptr<ScMemoryContext>& ctx_, ScAddr addr_): arc_addr(addr_), ms_context(ctx_)
{
    addr = ms_context->GetEdgeTarget(arc_addr);
    resolveModifiers();
    value_addr.Reset();
    if (operand_type == SCP_CONST)
    {
        value_addr = addr;
    }
    else
    {
        ScIterator3Ptr iter = ms_context->Iterator3(addr, ScType(sc_type_arc_access | sc_type_arc_pos | sc_type_const), ScType(0));
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
    ScIterator3Ptr iter = ms_context->Iterator3(addr, ScType(sc_type_arc_access | sc_type_arc_pos | sc_type_const), ScType(0));
    while (iter->Next())
    {
        ms_context->EraseElement(iter->Get(1));
    }
    value_addr.Reset();
}

void SCPOperand::SetValue(ScAddr value)
{
    value_addr = value;
    ms_context->CreateEdge(ScType(sc_type_arc_access | sc_type_arc_temp | sc_type_arc_pos | sc_type_const), addr, value);
}

ScAddr SCPOperand::CreateNodeOrLink()
{
    if (element_type.IsLink())
        value_addr = ms_context->CreateLink();
    else
        value_addr = ms_context->CreateNode(element_type);
    ms_context->CreateEdge(ScType(sc_type_arc_access | sc_type_arc_temp | sc_type_arc_pos | sc_type_const), addr, value_addr);
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
    ScIterator3Ptr iter = ms_context->Iterator3(ScType::NodeConst, ScType::EdgeAccessConstPosPerm, arc_addr);
    while (iter->Next())
    {
        ScAddr modifier = iter->Get(0);
        if (ms_context->HelperCheckEdge(Keynodes::order_role_relation, modifier, ScType::EdgeAccessConstPosPerm))
        {
            resolveOrder(modifier);
            continue;
        }

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
        if (modifier == Keynodes::rrel_link)
        {
            element_type = element_type | ScType::Link | ScType::Node;
            continue;
        }
        if (modifier == Keynodes::rrel_struct)
        {
            element_type = ScType(*element_type | sc_type_node_struct);
            continue;
        }
        if (modifier == Keynodes::rrel_norole_relation)
        {
            element_type = ScType(*element_type | sc_type_node_norole);
            continue;
        }
        if (modifier == Keynodes::rrel_role_relation)
        {
            element_type = ScType(*element_type | sc_type_node_role);
            continue;
        }
        if (modifier == Keynodes::rrel_class)
        {
            element_type = ScType(*element_type | sc_type_node_class);
            continue;
        }

        if (modifier == Keynodes::rrel_edge)
        {
            element_type = element_type | ScType::EdgeUCommon;
            continue;
        }
        if (modifier == Keynodes::rrel_common)
        {
            element_type = element_type | ScType::EdgeDCommon;
            continue;
        }

        //!TODO rrel_arc

        if (modifier == Keynodes::rrel_access)
        {
            element_type = element_type | ScType::EdgeAccess;
            continue;
        }
        if (modifier == Keynodes::rrel_temp)
        {
            element_type = ScType(*element_type | sc_type_arc_access | sc_type_arc_temp);
            continue;
        }
        if (modifier == Keynodes::rrel_perm)
        {
            element_type = ScType(*element_type | sc_type_arc_access | sc_type_arc_perm);
            continue;
        }
        if (modifier == Keynodes::rrel_pos)
        {
            element_type = ScType(*element_type | sc_type_arc_access | sc_type_arc_pos);
            continue;
        }
        if (modifier == Keynodes::rrel_neg)
        {
            element_type = ScType(*element_type | sc_type_arc_access | sc_type_arc_neg);
            continue;
        }
        if (modifier == Keynodes::rrel_fuz)
        {
            element_type = ScType(*element_type | sc_type_arc_access | sc_type_arc_fuz);
            continue;
        }
        if (modifier == Keynodes::rrel_pos_const_perm)
        {
            element_type = ScType(element_type | ScType::EdgeAccessConstPosPerm);
            continue;
        }
    }
}

}

