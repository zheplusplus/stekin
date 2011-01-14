#include <iostream>

#include "err-report.h"

static bool _has_error = false;

bool grammar::has_error()
{
    return _has_error;
}

void grammar::else_not_match_if(misc::pos_type const& else_pos)
{
    _has_error = true;
    std::cerr << else_pos << std::endl;
    std::cerr << "    ``else'' does not match an ``if''" << std::endl;
}

void grammar::if_already_match_else(misc::pos_type const& prev_else_pos, misc::pos_type const& this_else_pos)
{
    _has_error = true;
    std::cerr << this_else_pos << std::endl;
    std::cerr << "    another ``else'' already matches the ``if'' at " << prev_else_pos << std::endl;
}

void grammar::excessive_indent(misc::pos_type const& pos)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    excessive indentation" << std::endl;
}
