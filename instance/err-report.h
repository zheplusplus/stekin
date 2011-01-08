#ifndef __STACKENING_INSTANCE_ERROR_REPORT_H__
#define __STACKENING_INSTANCE_ERROR_REPORT_H__

#include <list>
#include <string>

#include "../misc/pos-type.h"

namespace inst {

    bool has_error();

    void var_not_def(misc::pos_type const& ref_lineno, std::string const& name);

    void binary_op_not_avai(misc::pos_type const& lineno
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

#endif /* __STACKENING_INSTANCE_ERROR_REPORT_H__ */
