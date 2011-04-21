#include <gtest/gtest.h>

#include "../yy-misc.h"
#include "../../test/phony-errors.h"
#include "../../flowcheck/node-base.h"
#include "../../test/common.h"

using namespace test;

TEST(Syntax, ErrTabAsIndent)
{
    yyparse();
    ASSERT_TRUE(error::has_error());
    std::vector<tab_as_ind_rec> recs = get_tab_as_ind_recs();
    ASSERT_EQ(3, recs.size());

    EXPECT_EQ(misc::position(2), recs[0].pos);
    EXPECT_EQ(misc::position(4), recs[1].pos);
    EXPECT_EQ(misc::position(7), recs[2].pos);
}
