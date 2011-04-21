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
    std::vector<BadIndentRec> recs = getBadIndents();
    ASSERT_EQ(3, recs.size());

    EXPECT_EQ(misc::position(1), recs[0].pos);
    EXPECT_EQ(misc::position(2), recs[1].pos);
    EXPECT_EQ(misc::position(3), recs[2].pos);
}
