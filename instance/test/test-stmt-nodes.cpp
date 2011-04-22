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
    inst::Branch(pos
               , std::move(util::mkptr(new cond_type_expr))
               , std::move(util::mkptr(new inst::Block))
               , std::move(util::mkptr(new inst::Block)));
    ASSERT_FALSE(error::hasError());
    inst::Branch(pos
               , std::move(util::mkptr(new bad_type_expr))
               , std::move(util::mkptr(new inst::Block))
               , std::move(util::mkptr(new inst::Block)));
    ASSERT_FALSE(error::hasError());

    inst::Branch(pos
               , std::move(util::mkptr(new void_type_expr))
               , std::move(util::mkptr(new inst::Block))
               , std::move(util::mkptr(new inst::Block)));
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, getCondNotBools().size());
    ASSERT_EQ(pos, getCondNotBools()[0].pos);
    ASSERT_EQ("void", getCondNotBools()[0].type_name);
}
