#ifndef __STACKENING_PARSER_ERROR_REPORT_H__
#define __STACKENING_PARSER_ERROR_REPORT_H__

#include "../misc/pos-type.h"

namespace parser {

    bool has_error();

    void tab_as_indent(misc::pos_type const& pos);
    void bad_indent(misc::pos_type const& pos);
    void invalid_char(misc::pos_type const& pos, int character);

}

#endif /* __STACKENING_PARSER_ERROR_REPORT_H__ */
