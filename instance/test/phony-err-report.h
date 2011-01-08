#ifndef __STACKENING_INSTANCE_TEST_PHONY_ERROR_REPORT_H__
#define __STACKENING_INSTANCE_TEST_PHONY_ERROR_REPORT_H__

#include <string>
#include <vector>

#include "../err-report.h"
#include "../../misc/pos-type.h"

namespace test {

    struct var_nondef_rec {
        misc::pos_type const ref_pos;
        std::string const name;

        var_nondef_rec(misc::pos_type const& rpos, std::string const& n)
            : ref_pos(rpos)
            , name(n)
        {}
    };

    struct na_binary_op_rec {
        misc::pos_type const pos;
        std::string const op_img;
        std::string const lhst_name;
        std::string const rhst_name;

        na_binary_op_rec(misc::pos_type const& ps
                       , std::string const& op
                       , std::string const& lhst
                       , std::string const& rhst)
            : pos(ps)
            , op_img(op)
            , lhst_name(lhst)
            , rhst_name(rhst)
        {}
    };

    struct na_pre_unary_op_rec {
        misc::pos_type const pos;
        std::string const op_img;
        std::string const rhst_name;

        na_pre_unary_op_rec(misc::pos_type const& ps, std::string const& op, std::string const& rhst)
            : pos(ps)
            , op_img(op)
            , rhst_name(rhst)
        {}
    };

    struct ret_type_conflict_rec {
        misc::pos_type const this_pos;
        std::string const prev_type_name;
        std::string const this_type_name;

        ret_type_conflict_rec(misc::pos_type const& tpos
                            , std::string const& prev_ret_type_name
                            , std::string const& this_ret_type_name)
            : this_pos(tpos)
            , prev_type_name(prev_ret_type_name)
            , this_type_name(this_ret_type_name)
        {}
    };

    struct ret_type_unresolvable_rec {
        std::string const name;
        int const arg_count;

        ret_type_unresolvable_rec(std::string const& n, int ac)
            : name(n)
            , arg_count(ac)
        {}
    };

    struct cond_not_bool_rec {
        misc::pos_type const pos;
        std::string const type_name;

        cond_not_bool_rec(misc::pos_type const& ps, std::string const& tname)
            : pos(ps)
            , type_name(tname)
        {}
    };

    void clear_err();

    std::vector<var_nondef_rec> get_nondefs();

    std::vector<na_binary_op_rec> get_na_binary_ops();
    std::vector<na_pre_unary_op_rec> get_na_pre_unary_ops();

    std::vector<ret_type_conflict_rec> get_ret_type_conflicts();
    std::vector<ret_type_unresolvable_rec> get_ret_type_unresolvables();

    std::vector<cond_not_bool_rec> get_cond_not_bools();

}

#endif /* __STACKENING_INSTANCE_TEST_PHONY_ERROR_REPORT_H__ */
