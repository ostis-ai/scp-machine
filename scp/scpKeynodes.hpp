/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once
#include <sc-memory/sc_keynodes.hpp>

namespace scp {
class Keynodes : public ScKeynodes
{
    public:

    static inline ScKeynode const scp_program{"scp_program"};

    static inline ScKeynode const agent_scp_program{"agent_scp_program"};

    static inline ScKeynode const platform_independent_abstract_sc_agent{"platform_independent_abstract_sc_agent"};

    static inline ScKeynode const abstract_sc_agent{"abstract_sc_agent"};

    static inline ScKeynode const nrel_sc_agent_program{"nrel_sc_agent_program"};

    static inline ScKeynode const nrel_inclusion{"nrel_inclusion"};

    static inline ScKeynode const nrel_sc_agent_action_class{"nrel_sc_agent_action_class"};

    static inline ScKeynode const nrel_primary_initiation_condition{"nrel_primary_initiation_condition"};

    static inline ScKeynode const action_scp_interpretation_request{"action_scp_interpretation_request"};

    static inline ScKeynode const action_initiated{"action_initiated"};

    static inline ScKeynode const action_finished{"action_finished"};

    static inline ScKeynode const action_finished_successfully{"action_finished_successfully"};

    static inline ScKeynode const action_finished_unsuccessfully{"action_finished_unsuccessfully"};

    static inline ScKeynode const action_finished_with_error{"action_finished_with_error"};

    static inline ScKeynode const scp_process{"scp_process"};

    static inline ScKeynode const abstract_scp_machine{"abstract_scp_machine"};

    static inline ScKeynode const nrel_decomposition_of_action{"nrel_decomposition_of_action"};

    static inline ScKeynode const nrel_scp_process{"nrel_scp_process"};

    static inline ScKeynode const rrel_key_sc_element{"rrel_key_sc_element"};

    static inline ScKeynode const nrel_authors{"nrel_authors"};

    static inline ScKeynode const nrel_system_identifier{"nrel_system_identifier"};

    static inline ScKeynode const active_action{"active_action"};

    static inline ScKeynode const active_sc_agent{"active_sc_agent", ScType::NodeConstClass};

    static inline ScKeynode const nrel_scp_var_value{"nrel_scp_var_value"};

// rrels
    static inline ScKeynode const nrel_then{"nrel_then"};

    static inline ScKeynode const nrel_else{"nrel_else"};

    static inline ScKeynode const nrel_goto{"nrel_goto"};

    static inline ScKeynode const nrel_error{"nrel_error"};

    static inline ScKeynode const nrel_result{"nrel_result"};

    static inline ScKeynode const rrel_in{"rrel_in"};

    static inline ScKeynode const rrel_out{"rrel_out"};

    static inline ScKeynode const rrel_fixed{"rrel_fixed"};

    static inline ScKeynode const rrel_assign{"rrel_assign"};

    static inline ScKeynode const rrel_set{"rrel_set"};

    static inline ScKeynode const rrel_set_1{"rrel_set_1"};

    static inline ScKeynode const rrel_set_2{"rrel_set_2"};

    static inline ScKeynode const rrel_set_3{"rrel_set_3"};

    static inline ScKeynode const rrel_set_4{"rrel_set_4"};

    static inline ScKeynode const rrel_set_5{"rrel_set_5"};

    static inline ScKeynode const rrel_erase{"rrel_erase"};

    static inline ScKeynode const rrel_const{"rrel_const"};

    static inline ScKeynode const rrel_var{"rrel_var"};

    static inline ScKeynode const rrel_scp_const{"rrel_scp_const"};

    static inline ScKeynode const rrel_scp_var{"rrel_scp_var"};

    static inline ScKeynode const rrel_node{"rrel_node"};

    static inline ScKeynode const rrel_link{"rrel_link"};

    static inline ScKeynode const rrel_arc{"rrel_arc"};

    static inline ScKeynode const rrel_edge{"rrel_edge"};

    static inline ScKeynode const rrel_pos{"rrel_pos"};

    static inline ScKeynode const rrel_neg{"rrel_neg"};

    static inline ScKeynode const rrel_fuz{"rrel_fuz"};

    static inline ScKeynode const rrel_temp{"rrel_temp"};

    static inline ScKeynode const rrel_perm{"rrel_perm"};

    static inline ScKeynode const rrel_access{"rrel_access"};

    static inline ScKeynode const rrel_common{"rrel_common"};

    static inline ScKeynode const rrel_pos_const_perm{"rrel_pos_const_perm"};

//Node types
    static inline ScKeynode const rrel_struct{"rrel_struct"};

    static inline ScKeynode const rrel_role_relation{"rrel_role_relation"};

    static inline ScKeynode const rrel_norole_relation{"rrel_norole_relation"};

    static inline ScKeynode const rrel_class{"rrel_class"};

// Events
    static inline ScKeynode const sc_event_remove_element{"sc_event_before_erase_element"};

    static inline ScKeynode const sc_event_add_output_arc{"sc_event_after_generate_incoming_arc"};

    static inline ScKeynode const sc_event_add_input_arc{"sc_event_after_generate_outgoing_arc"};

    static inline ScKeynode const sc_event_remove_output_arc{"sc_event_before_erase_outgoing_arc"};

    static inline ScKeynode const sc_event_remove_input_arc{"sc_event_before_erase_incoming_arc"};

    static inline ScKeynode const sc_event_content_changed{"sc_event_before_change_link_content"};

// Order relations
    static inline ScKeynode const rrel_1{"rrel_1"};

    static inline ScKeynode const rrel_2{"rrel_2"};

    static inline ScKeynode const rrel_3{"rrel_3"};

    static inline ScKeynode const rrel_4{"rrel_4"};

    static inline ScKeynode const rrel_5{"rrel_5"};

    static inline ScKeynode const rrel_6{"rrel_6"};

    static inline ScKeynode const rrel_7{"rrel_7"};

    static inline ScKeynode const rrel_8{"rrel_8"};

    static inline ScKeynode const rrel_9{"rrel_9"};

    static inline ScKeynode const rrel_10{"rrel_10"};

    static inline ScKeynode const order_role_relation{"order_role_relation"};

//Operators
    static inline ScKeynode const scp_operator_atomic_type{"scp_operator_atomic_type"};

    static inline ScKeynode const op_searchElStr3{"searchElStr3"};
    static inline ScKeynode const op_searchElStr5{"searchElStr5"};

    static inline ScKeynode const op_searchSetStr3{"searchSetStr3"};
    static inline ScKeynode const op_searchSetStr5{"searchSetStr5"};

    static inline ScKeynode const op_genEl{"genEl"};
    static inline ScKeynode const op_genElStr3{"genElStr3"};
    static inline ScKeynode const op_genElStr5{"genElStr5"};

    static inline ScKeynode const op_eraseEl{"eraseEl"};
    static inline ScKeynode const op_eraseElStr3{"eraseElStr3"};
    static inline ScKeynode const op_eraseElStr5{"eraseElStr5"};
    static inline ScKeynode const op_eraseSetStr3{"eraseSetStr3"};
    static inline ScKeynode const op_eraseSetStr5{"eraseSetStr5"};

    static inline ScKeynode const op_ifVarAssign{"ifVarAssign"};
    static inline ScKeynode const op_ifFormCont{"ifFormCont"};
    static inline ScKeynode const op_ifCoin{"ifCoin"};
    static inline ScKeynode const op_ifType{"ifType"};

    static inline ScKeynode const op_varAssign{"varAssign"};
    static inline ScKeynode const op_varErase{"varErase"};

    static inline ScKeynode const op_ifEq{"ifEq"};
    static inline ScKeynode const op_ifGr{"ifGr"};

    static inline ScKeynode const op_contAdd{"contAdd"};
    static inline ScKeynode const op_contDiv{"contDiv"};
    static inline ScKeynode const op_contMult{"contMult"};
    static inline ScKeynode const op_contSub{"contSub"};
    static inline ScKeynode const op_contPow{"contPow"};

    static inline ScKeynode const op_contLn{"contLn"};
    static inline ScKeynode const op_contSin{"contSin"};
    static inline ScKeynode const op_contCos{"contCos"};
    static inline ScKeynode const op_contTg{"contTg"};
    static inline ScKeynode const op_contASin{"contASin"};
    static inline ScKeynode const op_contACos{"contACos"};
    static inline ScKeynode const op_contATg{"contATg"};

    static inline ScKeynode const op_contDivInt{"contDivInt"};
    static inline ScKeynode const op_contDivRem{"contDivRem"};

    static inline ScKeynode const op_contStringConcat{"contStringConcat"};


    static inline ScKeynode const op_stringIfEq{"stringIfEq"};
    static inline ScKeynode const op_stringIfGr{"stringIfGr"};
    static inline ScKeynode const op_stringSplit{"stringSplit"};
    static inline ScKeynode const op_stringLen{"stringLen"};
    static inline ScKeynode const op_stringSub{"stringSub"};
    static inline ScKeynode const op_stringSlice{"stringSlice"};
    static inline ScKeynode const op_stringStartsWith{"stringStartsWith"};
    static inline ScKeynode const op_stringEndsWith{"stringEndsWith"};
    static inline ScKeynode const op_stringReplace{"stringReplace"};
    static inline ScKeynode const op_stringToUpperCase{"stringToUpperCase"};
    static inline ScKeynode const op_stringToLowerCase{"stringToLowerCase"};


    static inline ScKeynode const op_contAssign{"contAssign"};
    static inline ScKeynode const op_contErase{"contErase"};

    static inline ScKeynode const op_print{"print"};
    static inline ScKeynode const op_printNl{"printNl"};
    static inline ScKeynode const op_printEl{"printEl"};

    static inline ScKeynode const op_sys_search{"sys_search"};
    static inline ScKeynode const op_sys_gen{"sys_gen"};

    static inline ScKeynode const op_call{"call"};
    static inline ScKeynode const op_waitReturn{"waitReturn"};
    static inline ScKeynode const op_waitReturnSet{"waitReturnSet"};
    static inline ScKeynode const op_return{"return"};

    static inline ScKeynode const op_sys_wait{"sys_wait"};

    static inline ScKeynode const op_syncronize{"syncronize"};

};

}
