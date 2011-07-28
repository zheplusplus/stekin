#include <gtest/gtest.h>

#include "test-common.h"
#include "../built-in.h"
#include "../expr-nodes.h"
#include "../../misc/platform.h"

using namespace test;

typedef InstanceTest BuiltInTest;

TEST_F(BuiltInTest, WriteExpr)
{
    inst::WriteExpr w0(util::mkptr(new inst::IntLiteral(2337)));
    inst::WriteExpr w1(util::mkptr(new inst::BoolLiteral(false)));

    w0.write();
    w1.write();

    DataTree::expectOne()
        (WRITE_BEGIN)
            (INTEGER, "2337")
        (WRITE_END)
        (WRITE_BEGIN)
            (BOOLEAN, "false")
        (WRITE_END)
    ;
}
