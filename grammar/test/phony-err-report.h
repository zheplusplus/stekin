#ifndef __STACKENING_GRAMMAR_PHONY_ERROR_REPORT_H__
#define __STACKENING_GRAMMAR_PHONY_ERROR_REPORT_H__

#include <vector>

#include "../err-report.h"

namespace test {

    struct else_not_match_rec {
        misc::pos_type const pos;

        explicit else_not_match_rec(misc::pos_type const& ps)
            : pos(ps)
        {}
    };

    struct if_matched_rec {
        misc::pos_type const prev_pos;
        misc::pos_type const this_pos;

        if_matched_rec(misc::pos_type const& ppos, misc::pos_type const& tpos)
            : prev_pos(ppos)
            , this_pos(tpos)
        {}
    };

    struct excess_ind_rec {
        misc::pos_type const pos;

        explicit excess_ind_rec(misc::pos_type const& ps)
            : pos(ps)
        {}
    };

    void clear_err();

    std::vector<else_not_match_rec> get_else_not_matches();
    std::vector<if_matched_rec> get_if_matcheds();
    std::vector<excess_ind_rec> get_excess_inds();

}

#endif /* __STACKENING_GRAMMAR_PHONY_ERROR_REPORT_H__ */