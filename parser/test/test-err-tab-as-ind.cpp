#include <gtest/gtest.h>

#include "../yy-misc.h"
#include "../../test/phony-errors.h"
#include "../../flowcheck/node-base.h"

using namespace test;

TEST(Syntax, ErrTabAsIndent)
{
    yyparse();
    ASSERT_TRUE(error::has_error());
    std::vector<tab_as_ind_rec> recs = get_tab_as_ind_recs();
    ASSERT_EQ(3, recs.size());

    EXPECT_EQ(misc::pos_type(2), recs[0].pos);
    EXPECT_EQ(misc::pos_type(4), recs[1].pos);
    EXPECT_EQ(misc::pos_type(7), recs[2].pos);
}
