#include <gtest/gtest.h>

#include "test-common.h"
#include "../expr-nodes.h"
#include "../../util/string.h"
#include "../../misc/platform.h"

using namespace test;

typedef InstanceTest ExprNodesTest;

TEST_F(ExprNodesTest, Literals)
{
    inst::IntLiteral i0(0);
    inst::IntLiteral i1(0x7fffffffffffffff);

    i0.write();
    i1.write();

    DataTree::expectOne()
        (INTEGER, "0")
        (INTEGER, util::str(platform::int_type(0x7fffffffffffffff)))
    ;
}
