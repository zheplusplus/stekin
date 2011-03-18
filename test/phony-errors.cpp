#include <list>

#include "phony-errors.h"

using namespace test;

static std::list<tab_as_ind_rec> tab_as_ind_recs;

static std::list<bad_indent_rec> bad_indent_recs;
static std::list<inv_char_rec> inv_char_recs;

static std::list<else_not_match_rec> else_not_matches_recs;
static std::list<if_matched_rec> if_matcheds_recs;
static std::list<excess_ind_rec> excess_ind_recs;
static std::list<flow_terminated_rec> flow_terminated_recs;

static std::list<func_forbidden_rec> forbidden_func_recs;
static std::list<forbid_def_rec> forbid_var_def_recs;
static std::list<var_redef_rec> local_redefs;
static std::list<invalid_ref_rec> invalid_refs;
static std::list<func_redef_rec> local_func_redefs;
static std::list<func_redef_rec> func_shadow_external_recs;
static std::list<func_nondef_rec> func_nondefs;

static std::list<ret_type_conflict_rec> ret_type_conflict_recs;
static std::list<ret_type_unresolvable_rec> ret_type_unresolvable_recs;
static std::list<cond_not_bool_rec> cond_not_bool_recs;
static std::list<var_nondef_rec> var_nondefs;
static std::list<na_binary_op_rec> na_binary_ops;
static std::list<na_pre_unary_op_rec> na_pre_unary_ops;

static bool has_err = false;

void test::clear_err()
{
    has_err = false;

    tab_as_ind_recs.clear();
    bad_indent_recs.clear();
    inv_char_recs.clear();

    else_not_matches_recs.clear();
    if_matcheds_recs.clear();
    excess_ind_recs.clear();
    flow_terminated_recs.clear();

    forbidden_func_recs.clear();
    forbid_var_def_recs.clear();
    local_redefs.clear();
    invalid_refs.clear();
    local_func_redefs.clear();
    func_shadow_external_recs.clear();
    func_nondefs.clear();

    var_nondefs.clear();
    na_binary_ops.clear();
    na_pre_unary_ops.clear();
    ret_type_conflict_recs.clear();
    ret_type_unresolvable_recs.clear();
    cond_not_bool_recs.clear();
}

void yyerror(std::string const&)
{
    has_err = true;
}

bool error::has_error()
{
    return has_err;
}

void error::tab_as_indent(misc::pos_type const& pos)
{
    has_err = true;
    tab_as_ind_recs.push_back(tab_as_ind_rec(pos));
}

void error::bad_indent(misc::pos_type const& pos)
{
    has_err = true;
    bad_indent_recs.push_back(bad_indent_rec(pos));
}

void error::invalid_char(misc::pos_type const& pos, int character)
{
    has_err = true;
    inv_char_recs.push_back(inv_char_rec(pos, character));
}

void error::else_not_match_if(misc::pos_type const& pos)
{
    has_err = true;
    else_not_matches_recs.push_back(else_not_match_rec(pos));
}

void error::if_already_match_else(misc::pos_type const& prev_else_pos, misc::pos_type const& this_else_pos)
{
    has_err = true;
    if_matcheds_recs.push_back(if_matched_rec(prev_else_pos, this_else_pos));
}

void error::excessive_indent(misc::pos_type const& pos)
{
    has_err = true;
    excess_ind_recs.push_back(excess_ind_rec(pos));
}

void error::flow_terminated(misc::pos_type const& this_pos, misc::pos_type const& prev_pos)
{
    has_err = true;
    flow_terminated_recs.push_back(flow_terminated_rec(this_pos, prev_pos));
}

void error::forbid_def_func(misc::pos_type const& pos, std::string const& name)
{
    has_err = true;
    forbidden_func_recs.push_back(func_forbidden_rec(pos, name));
}

void error::forbid_def_var(misc::pos_type const& pos, std::string const& name)
{
    has_err = true;
    forbid_var_def_recs.push_back(forbid_def_rec(pos, name));
}

void error::var_already_in_local(misc::pos_type const& prev_def_pos
                               , misc::pos_type const& this_def_pos
                               , std::string const& name)
{
    has_err = true;
    local_redefs.push_back(var_redef_rec(prev_def_pos, this_def_pos, name));
}

void error::var_ref_before_def(misc::pos_type const& def_pos
                             , std::list<misc::pos_type> const& ref_positions
                             , std::string const& name)
{
    has_err = true;
    invalid_refs.push_back(invalid_ref_rec(ref_positions.begin(), ref_positions.end(), def_pos, name));
}

void error::func_already_in_local(misc::pos_type const& prev_def_pos
                                , misc::pos_type const& this_def_pos
                                , std::string const& name
                                , int param_count)
{
    has_err = true;
    local_func_redefs.push_back(func_redef_rec(prev_def_pos, this_def_pos, name, param_count));
}

void error::func_shadow_external(misc::pos_type const& prev_def_pos
                               , misc::pos_type const& this_def_pos
                               , std::string const& name
                               , int param_count)
{
    has_err = true;
    func_shadow_external_recs.push_back(func_redef_rec(prev_def_pos, this_def_pos, name, param_count));
}

void error::func_not_def(misc::pos_type const& call_pos, std::string const& name, int param_count)
{
    has_err = true;
    func_nondefs.push_back(func_nondef_rec(call_pos, name, param_count));
}

void error::var_not_def(misc::pos_type const& ref_pos, std::string const& name)
{
    has_err = true;
    var_nondefs.push_back(var_nondef_rec(ref_pos, name));
}

void error::binary_op_not_avai(misc::pos_type const& pos
                             , std::string const& op_img
                             , std::string const& lhst
                             , std::string const& rhst)
{
    has_err = true;
    na_binary_ops.push_back(na_binary_op_rec(pos, op_img, lhst, rhst));
}

void error::pre_unary_op_not_avai(misc::pos_type const& pos, std::string const& op_img, std::string const& rhst)
{
    has_err = true;
    na_pre_unary_ops.push_back(na_pre_unary_op_rec(pos, op_img, rhst));
}

void error::conflict_return_type(misc::pos_type const& this_pos
                               , std::string const& prev_type_name
                               , std::string const& this_type_name)
{
    has_err = true;
    ret_type_conflict_recs.push_back(ret_type_conflict_rec(this_pos, prev_type_name, this_type_name));
}

void error::func_ret_type_unresolvable(std::string const& name, int arg_count)
{
    has_err = true;
    ret_type_unresolvable_recs.push_back(ret_type_unresolvable_rec(name, arg_count));
}

void error::cond_not_bool(misc::pos_type const& pos, std::string const& actual_type)
{
    has_err = true;
    cond_not_bool_recs.push_back(cond_not_bool_rec(pos, actual_type));
}

std::vector<tab_as_ind_rec> test::get_tab_as_ind_recs()
{
    return std::vector<tab_as_ind_rec>(tab_as_ind_recs.begin(), tab_as_ind_recs.end());
}

std::vector<bad_indent_rec> test::get_bad_indent_recs()
{
    return std::vector<bad_indent_rec>(bad_indent_recs.begin(), bad_indent_recs.end());
}

std::vector<inv_char_rec> test::get_inv_char_recs()
{
    return std::vector<inv_char_rec>(inv_char_recs.begin(), inv_char_recs.end());
}

std::vector<else_not_match_rec> test::get_else_not_matches()
{
    return std::vector<else_not_match_rec>(else_not_matches_recs.begin(), else_not_matches_recs.end());
}

std::vector<if_matched_rec> test::get_if_matcheds()
{
    return std::vector<if_matched_rec>(if_matcheds_recs.begin(), if_matcheds_recs.end());
}

std::vector<excess_ind_rec> test::get_excess_inds()
{
    return std::vector<excess_ind_rec>(excess_ind_recs.begin(), excess_ind_recs.end());
}

std::vector<func_forbidden_rec> test::get_forbidden_funcs()
{
    return std::vector<func_forbidden_rec>(forbidden_func_recs.begin(), forbidden_func_recs.end());
}

std::vector<forbid_def_rec> test::get_forbid_var_defs()
{
    return std::vector<forbid_def_rec>(forbid_var_def_recs.begin(), forbid_var_def_recs.end());
}

std::vector<var_redef_rec> test::get_local_redefs()
{
    return std::vector<var_redef_rec>(local_redefs.begin(), local_redefs.end());
}

std::vector<invalid_ref_rec> test::get_invalid_refs()
{
    return std::vector<invalid_ref_rec>(invalid_refs.begin(), invalid_refs.end());
}

std::vector<func_redef_rec> test::get_local_func_redefs()
{
    return std::vector<func_redef_rec>(local_func_redefs.begin(), local_func_redefs.end());
}

std::vector<func_redef_rec> test::get_func_shadow_external()
{
    return std::vector<func_redef_rec>(func_shadow_external_recs.begin(), func_shadow_external_recs.end());
}

std::vector<func_nondef_rec> test::get_func_nondefs()
{
    return std::vector<func_nondef_rec>(func_nondefs.begin(), func_nondefs.end());
}

std::vector<var_nondef_rec> test::get_nondefs()
{
    return std::vector<var_nondef_rec>(var_nondefs.begin(), var_nondefs.end());
}

std::vector<na_binary_op_rec> test::get_na_binary_ops()
{
    return std::vector<na_binary_op_rec>(na_binary_ops.begin(), na_binary_ops.end());
}

std::vector<na_pre_unary_op_rec> test::get_na_pre_unary_ops()
{
    return std::vector<na_pre_unary_op_rec>(na_pre_unary_ops.begin(), na_pre_unary_ops.end());
}

std::vector<ret_type_conflict_rec> test::get_ret_type_conflicts()
{
    return std::vector<ret_type_conflict_rec>(ret_type_conflict_recs.begin(), ret_type_conflict_recs.end());
}

std::vector<ret_type_unresolvable_rec> test::get_ret_type_unresolvables()
{
    return std::vector<ret_type_unresolvable_rec>(ret_type_unresolvable_recs.begin()
                                                , ret_type_unresolvable_recs.end());
}

std::vector<cond_not_bool_rec> test::get_cond_not_bools()
{
    return std::vector<cond_not_bool_rec>(cond_not_bool_recs.begin(), cond_not_bool_recs.end());
}
