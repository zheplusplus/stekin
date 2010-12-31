#ifndef __STACKENING_PARSER_PHONY_ERROR_REPORT_H__
#define __STACKENING_PARSER_PHONY_ERROR_REPORT_H__

#include <vector>

#include "../err-report.h"

namespace test {

    struct tab_as_ind_rec {
        explicit tab_as_ind_rec(misc::pos_type const& ps)
            : pos(ps)
        {}

        misc::pos_type const pos;
    };

    struct bad_indent_rec {
        explicit bad_indent_rec(misc::pos_type const& ps)
            : pos(ps)
        {}

        misc::pos_type const pos;
    };

    struct inv_char_rec {
        inv_char_rec(misc::pos_type const& ps, int ch)
            : pos(ps)
            , character(ch)
        {}

        misc::pos_type const pos;
        int const character;
    };

    std::vector<tab_as_ind_rec> get_tab_as_ind_recs();
    std::vector<bad_indent_rec> get_bad_indent_recs();
    std::vector<inv_char_rec> get_inv_char_recs();

}

#endif /* __STACKENING_PARSER_PHONY_ERROR_REPORT_H__ */
