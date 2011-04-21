#ifndef __STEKIN_TEST_PHONY_WARNINGS_H__
#define __STEKIN_TEST_PHONY_WARNINGS_H__

#include <vector>

#include "../report/warnings.h"

namespace test {

    struct consq_branch_terminated_rec {
        explicit consq_branch_terminated_rec(misc::position const& ps)
            : pos(ps)
        {}

        misc::position const pos;
    };

    struct alter_branch_terminated_rec {
        explicit alter_branch_terminated_rec(misc::position const& ps)
            : pos(ps)
        {}

        misc::position const pos;
    };

    struct both_branches_terminated_rec {
        both_branches_terminated_rec(misc::position const& consq_ps, misc::position const& alter_ps)
            : consq_pos(consq_ps)
            , alter_pos(alter_ps)
        {}

        misc::position const consq_pos;
        misc::position const alter_pos;
    };

    void clear_warn();

    std::vector<consq_branch_terminated_rec> get_consq_branch_terminated();
    std::vector<alter_branch_terminated_rec> get_alter_branch_terminated();
    std::vector<both_branches_terminated_rec> get_both_branches_terminated();

}

#endif /* __STEKIN_TEST_PHONY_WARNINGS_H__ */
