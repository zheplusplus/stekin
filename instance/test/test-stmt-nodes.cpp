#include <gtest/gtest.h>

#include "test-common.h"
#include "../stmt-nodes.h"
#include "../block.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"

using namespace test;

typedef instance_test StmtNodesTest;

TEST_F(StmtNodesTest, BranchConditionTypeCheck)
{
    misc::position pos(1);
    inst::branch(pos
               , std::move(util::mkptr(new cond_type_expr))
               , std::move(util::mkptr(new inst::Block))
               , std::move(util::mkptr(new inst::Block)));
    ASSERT_FALSE(error::has_error());
    inst::branch(pos
               , std::move(util::mkptr(new bad_type_expr))
               , std::move(util::mkptr(new inst::Block))
               , std::move(util::mkptr(new inst::Block)));
    ASSERT_FALSE(error::has_error());

    inst::branch(pos
               , std::move(util::mkptr(new void_type_expr))
               , std::move(util::mkptr(new inst::Block))
               , std::move(util::mkptr(new inst::Block)));
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_cond_not_bools().size());
    ASSERT_EQ(pos, get_cond_not_bools()[0].pos);
    ASSERT_EQ("void", get_cond_not_bools()[0].type_name);
}
