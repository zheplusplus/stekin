#include <gtest/gtest.h>

#include "phony-err-report.h"
#include "../yy-misc.h"

TEST(Syntax, Empty)
{
    yyparse();
    ASSERT_FALSE(parser::has_error());
}
