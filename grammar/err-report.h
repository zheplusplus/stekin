#ifndef __STACKENING_GRAMMAR_ERROR_REPORT_H__
#define __STACKENING_GRAMMAR_ERROR_REPORT_H__

#include "../misc/pos-type.h"

namespace grammar {

    bool has_error();

    void else_not_match_if(misc::pos_type const& else_pos);
    void if_already_match_else(misc::pos_type const& prev_else_pos, misc::pos_type const& this_else_pos);
    void excessive_indent(misc::pos_type const& pos);

}

#endif /* __STACKENING_GRAMMAR_ERROR_REPORT_H__ */
