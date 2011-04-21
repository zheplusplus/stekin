#include <gtest/gtest.h>

#include "../yy-misc.h"
#include "../../test/phony-errors.h"
#include "../../flowcheck/node-base.h"

TEST(Syntax, Empty)
{
    yyparse();
    ASSERT_FALSE(error::hasError());
}
