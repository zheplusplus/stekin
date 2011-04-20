#ifndef __STEKIN_REPORT_WARNINGS_H__
#define __STEKIN_REPORT_WARNINGS_H__

#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace warning {

    void one_or_two_branches_terminated(util::sref<misc::pos_type const> consq_term_pos_or_null_if_not_term
                                      , util::sref<misc::pos_type const> alter_term_pos_or_null_if_not_term);

}

#endif /* __STEKIN_REPORT_WARNINGS_H__ */
