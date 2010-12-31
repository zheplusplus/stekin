#include <gtest/gtest.h>

#include "phony-err-report.h"
#include "../yy-misc.h"

using namespace test;

TEST(Syntax, BadIndentation)
{
    yyparse();
    ASSERT_TRUE(parser::has_error());
    std::vector<bad_indent_rec> recs = get_bad_indent_recs();
    ASSERT_EQ(3, recs.size());

    EXPECT_EQ(misc::pos_type(1), recs[0].pos);
    EXPECT_EQ(misc::pos_type(2), recs[1].pos);
    EXPECT_EQ(misc::pos_type(3), recs[2].pos);
}
