#include <gtest/gtest.h>

#include "../yy-misc.h"
#include "../../test/phony-errors.h"

TEST(Syntax, Empty)
{
    yyparse();
    ASSERT_FALSE(error::has_error());
}
