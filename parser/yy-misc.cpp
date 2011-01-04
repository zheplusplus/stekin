#include <iostream>
#include <deque>

#include "yy-misc.h"

int yywrap(void)
{
    return 1;
}

grammar::clause_builder parser::builder;
int parser::last_indent = 0;

misc::pos_type parser::here()
{
    return misc::pos_type(yylineno);
}

misc::pos_type parser::here(int lineno)
{
    return misc::pos_type(lineno);
}
