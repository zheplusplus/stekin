#include <iostream>
#include <deque>

#include "yy-misc.h"

int yywrap(void)
{
    return 1;
}

grammar::clause_builder parser::builder;

misc::pos_type parser::here()
{
    return misc::pos_type(yylineno);
}
