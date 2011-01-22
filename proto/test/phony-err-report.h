#ifndef __STACKENING_PROTO_TEST_PHONY_ERROR_REPORT_H__
#define __STACKENING_PROTO_TEST_PHONY_ERROR_REPORT_H__

#include <vector>
#include <string>
#include <set>

#include "../err-report.h"

namespace test {

    struct var_redef_rec {
        misc::pos_type const prev_pos;
        misc::pos_type const this_pos;
        std::string const name;

        var_redef_rec(misc::pos_type const& ppos, misc::pos_type const& tpos, std::string const& n)
            : prev_pos(ppos)
            , this_pos(tpos)
            , name(n)
        {}
    };

    struct invalid_ref_rec {
        std::vector<misc::pos_type> const ref_positions;
        misc::pos_type const def_pos;
        std::string const name;

        template <typename _LineNoIterator>
        invalid_ref_rec(_LineNoIterator pos_begin
                      , _LineNoIterator pos_end
                      , misc::pos_type def_ps
                      , std::string const& n)
            : ref_positions(pos_begin, pos_end)
            , def_pos(def_ps)
            , name(n)
        {}
    };

    struct func_redef_rec {
        misc::pos_type const prev_def_pos;
        misc::pos_type const this_def_pos;
        std::string const name;
        int const param_count;

        func_redef_rec(misc::pos_type prev_def_ps
                     , misc::pos_type this_def_ps
                     , std::string const& n
                     , int pc)
            : prev_def_pos(prev_def_ps)
            , this_def_pos(this_def_ps)
            , name(n)
            , param_count(pc)
        {}
    };

    struct func_nondef_rec {
        misc::pos_type const call_pos;
        std::string const name;
        int const param_count;

        func_nondef_rec(misc::pos_type call_ps, std::string const& n, int pc)
            : call_pos(call_ps)
            , name(n)
            , param_count(pc)
        {}
    };

    struct forbid_def_rec {
        misc::pos_type const pos;
        std::string const name;

        forbid_def_rec(misc::pos_type ps, std::string const& n)
            : pos(ps)
            , name(n)
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

    void clear_err();

    std::vector<forbid_def_rec> get_forbid_var_defs();
    std::vector<var_redef_rec> get_local_redefs();
    std::vector<invalid_ref_rec> get_invalid_refs();

    std::vector<forbid_def_rec> get_forbid_func_defs();
    std::vector<func_redef_rec> get_local_func_redefs();
    std::vector<func_redef_rec> get_func_shadow_external();
    std::vector<func_nondef_rec> get_func_nondefs();

    std::vector<ret_type_unresolvable_rec> get_ret_type_unresolvables();

}

#endif /* __STACKENING_PROTO_TEST_PHONY_ERROR_REPORT_H__ */