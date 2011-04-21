#include <iostream>
#include <deque>

#include "yy-misc.h"
#include "../flowcheck/node-base.h"

int yywrap(void)
{
    return 1;
}

grammar::clause_builder parser::builder;
int parser::last_indent = 0;

misc::position parser::here()
{
    return misc::position(yylineno);
}

misc::position parser::here(int lineno)
{
    return misc::position(lineno);
}
