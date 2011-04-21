#include <gtest/gtest.h>

#include "../yy-misc.h"
#include "../../test/phony-errors.h"
#include "../../flowcheck/node-base.h"
#include "../../test/common.h"

using namespace test;

TEST(Syntax, BadIndentation)
{
    yyparse();
    ASSERT_TRUE(error::has_error());
    std::vector<bad_indent_rec> recs = get_bad_indent_recs();
    ASSERT_EQ(3, recs.size());

    EXPECT_EQ(misc::position(1), recs[0].pos);
    EXPECT_EQ(misc::position(2), recs[1].pos);
    EXPECT_EQ(misc::position(3), recs[2].pos);
}
