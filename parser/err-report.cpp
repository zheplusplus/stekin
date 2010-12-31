#include <iostream>

#include "err-report.h"
#include "yy-misc.h"

static bool _has_error = false;

bool parser::has_error()
{
    return _has_error;
}

void yyerror(std::string const& msg)
{
    _has_error = true;
    std::cerr << "Line " << yylineno << ":" << std::endl;
    std::cerr << "    " <<  msg << std::endl;
}

void tab_as_indent(misc::pos_type const& pos)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    use tab as indent is forbidden." << std::endl;
}

void bad_indent(misc::pos_type const& pos)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    indent not exactly 4 spaces." << std::endl;
}

void invalid_char(misc::pos_type const& pos, int character)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    invalid character " << char(character)
              << " (decimal value: " << character << ')' << std::endl;
}
