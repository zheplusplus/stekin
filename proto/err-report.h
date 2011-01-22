#ifndef __STACKENING_PROTO_ERROR_REPORT_H__
#define __STACKENING_PROTO_ERROR_REPORT_H__

#include <list>
#include <string>
#include "../misc/pos-type.h"

namespace proto {

    bool has_error();

    void forbid_def_var(misc::pos_type const& pos, std::string const& name);
    void var_already_in_local(misc::pos_type const& prev_def_pos
                            , misc::pos_type const& this_def_pos
                            , std::string const& var_name);
    void var_ref_before_def(misc::pos_type const& def_pos
                          , std::list<misc::pos_type> const& ref_positions
                          , std::string const& var_name);

    void forbid_def_func(misc::pos_type const& pos, std::string const& name);
    void func_already_in_local(misc::pos_type const& prev_def_pos
                             , misc::pos_type const& this_def_pos
                             , std::string const& name
                             , int param_count);
    void func_shadow_external(misc::pos_type const& prev_def_pos
                            , misc::pos_type const& this_def_pos
                            , std::string const& name
                            , int param_count);
    void func_not_def(misc::pos_type const& ref_pos, std::string const& name, int param_count);

    void func_ret_type_unresolvable(std::string const& name, int arg_count);

}

#endif /* __STACKENING_PROTO_ERROR_REPORT_H__ */
