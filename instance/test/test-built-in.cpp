#include <gtest/gtest.h>

#include "test-common.h"
#include "../built-in.h"
#include "../expr-nodes.h"

using namespace test;

typedef InstanceTest BuiltInTest;

TEST_F(BuiltInTest, WriterExpr)
{
    inst::WriterExpr w0(util::mkptr(new inst::IntLiteral(2337)));
    inst::WriterExpr w1(util::mkptr(new inst::BoolLiteral(false)));

    w0.write();
    w1.write();

    DataTree::expectOne()
        (WRITER_BEGIN)
            (INTEGER, "2337")
        (WRITER_END)
        (WRITER_BEGIN)
            (BOOLEAN, "false")
        (WRITER_END)
    ;
}
