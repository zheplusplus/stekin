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
    inst::FloatLiteral f0(3.14);
    inst::FloatLiteral f1(-2.72);
    inst::BoolLiteral b0(false);
    inst::BoolLiteral b1(true);

    i0.write();
    i1.write();
    f0.write();
    f1.write();
    b0.write();
    b1.write();

    DataTree::expectOne()
        (INTEGER, "0")
        (INTEGER, util::str(platform::int_type(0x7fffffffffffffff)))
        (FLOAT, "3.14")
        (FLOAT, "-2.72")
        (BOOLEAN, "false")
        (BOOLEAN, "true")
    ;
}

TEST_F(ExprNodesTest, Reference)
{
    inst::Reference r0(util::mkptr(new inst::VoidPrimitive), inst::Address(0, 0));
    inst::Reference r1(util::mkptr(new inst::FloatPrimitive), inst::Address(0, 8));
    inst::Reference r2(util::mkptr(new inst::IntPrimitive), inst::Address(0, 16));
    inst::Reference r3(util::mkptr(new inst::BoolPrimitive), inst::Address(0, 24));

    std::vector<util::sptr<inst::Type const>> types;
    types.push_back(util::mkptr(new inst::FloatPrimitive));
    types.push_back(util::mkptr(new inst::IntPrimitive));
    inst::Reference r4(util::mkptr(new inst::ClosureType(16, std::move(types)))
                     , inst::Address(1, 0));

    std::vector<util::sptr<inst::Type const>> sub_types;
    sub_types.push_back(util::mkptr(new inst::BoolPrimitive));
    sub_types.push_back(util::mkptr(new inst::BoolPrimitive));
    types.push_back(util::mkptr(new inst::IntPrimitive));
    types.push_back(util::mkptr(new inst::ClosureType(2, std::move(sub_types))));
    inst::Reference r5(util::mkptr(new inst::ClosureType(6, std::move(types)))
                     , inst::Address(1, 16));

    r0.write();
    r1.write();
    r2.write();
    r3.write();
    r4.write();
    r5.write();

    DataTree::expectOne()
        (REFERENCE, "void", 0, 0)
        (REFERENCE, "float", 0, 8)
        (REFERENCE, "int", 0, 16)
        (REFERENCE, "bool", 0, 24)
        (REFERENCE, "16", 1, 0)
        (REFERENCE, "6", 1, 16)
    ;
}
