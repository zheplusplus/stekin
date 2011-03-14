#ifndef __STACKENING_REPORT_ERRORS_H__
#define __STACKENING_REPORT_ERRORS_H__

#include <list>
#include <string>

#include "../misc/pos-type.h"

namespace error {

    bool has_error();

    void tab_as_indent(misc::pos_type const& pos);
    void bad_indent(misc::pos_type const& pos);
    void invalid_char(misc::pos_type const& pos, int character);

    void else_not_match_if(misc::pos_type const& else_pos);
    void if_already_match_else(misc::pos_type const& prev_else_pos, misc::pos_type const& this_else_pos);
    void excessive_indent(misc::pos_type const& pos);

    void flow_terminated(misc::pos_type const& this_pos, misc::pos_type const& prev_pos);

    void forbid_def_func(misc::pos_type const& pos, std::string const& name);
    void forbid_def_var(misc::pos_type const& pos, std::string const& name);

    void var_already_in_local(misc::pos_type const& prev_def_pos
                            , misc::pos_type const& this_def_pos
                            , std::string const& var_name);
    void var_ref_before_def(misc::pos_type const& def_pos
                          , std::list<misc::pos_type> const& ref_positions
                          , std::string const& name);

    void func_already_in_local(misc::pos_type const& prev_def_pos
                             , misc::pos_type const& this_def_pos
                             , std::string const& name
                             , int param_count);
    void func_shadow_external(misc::pos_type const& prev_def_pos
                            , misc::pos_type const& this_def_pos
                            , std::string const& name
                            , int param_count);
    void func_not_def(misc::pos_type const& ref_pos, std::string const& name, int param_count);

    void var_not_def(misc::pos_type const& ref_pos, std::string const& name);

    void binary_op_not_avai(misc::pos_type const& pos
                          , std::string const& op_img
                          , std::string const& lhst_name
                          , std::string const& rhst_name);
    void pre_unary_op_not_avai(misc::pos_type const& pos
                             , std::string const& op_img
                             , std::string const& rhst_name);

    void conflict_return_type(misc::pos_type const& this_pos
                            , std::string const& prev_ret_type_name
                            , std::string const& this_ret_type_name);

    void func_ret_type_unresolvable(std::string const& name, int arg_count);

    void cond_not_bool(misc::pos_type const& pos, std::string const& actual_type);

}

#endif /* __STACKENING_REPORT_ERRORS_H__ */
