#include <gtest/gtest.h>

#include "test-common.h"
#include "../stmt-nodes.h"
#include "../expr-nodes.h"
#include "../block.h"
#include "../types.h"

using namespace test;

typedef InstanceTest StmtNodesTest;

TEST_F(StmtNodesTest, Arithmetics)
{
    inst::Arithmetics a0(0, util::mkptr(new inst::BoolLiteral(false)));
    inst::Arithmetics a1(0, util::mkptr(new inst::FloatLiteral(22.38)));

    a0.write();
    a1.write();

    DataTree::expectOne()
        (BOOLEAN, "false")
        (END_OF_STATEMENT)
        (FLOAT, "22.38")
        (END_OF_STATEMENT)
    ;
}

TEST_F(StmtNodesTest, Branch)
{
    inst::Branch b0(0
                  , util::mkptr(new inst::BoolLiteral(true))
                  , util::mkptr(new inst::Arithmetics(0, util::mkptr(new inst::IntLiteral(0))))
                  , util::mkptr(new inst::Arithmetics(0, util::mkptr(new inst::IntLiteral(1)))));

    b0.write();

    DataTree::expectOne()
        (BRANCH_IF)
            (EXPRESSION_BEGIN)
            (BOOLEAN, "true")
            (EXPRESSION_END)
            (INTEGER, "0")
            (END_OF_STATEMENT)
        (BRANCH_ELSE)
            (INTEGER, "1")
            (END_OF_STATEMENT)
    ;
}

TEST_F(StmtNodesTest, Initialization)
{
    inst::Initialization i0(1
                          , 0
                          , util::mkptr(new inst::FloatLiteral(21.17))
                          , util::mkptr(new inst::FloatPrimitive));

    i0.write();

    DataTree::expectOne()
        (INITIALIZE_THIS_LEVEL, "float", 0)
            (EXPRESSION_BEGIN)
            (FLOAT, "21.17")
            (EXPRESSION_END)
        (END_OF_STATEMENT)
    ;
}

TEST_F(StmtNodesTest, Returns)
{
    inst::Return r0(1, util::mkptr(new inst::BoolLiteral(false)));
    inst::ReturnNothing r1;

    r0.write();
    r1.write();

    DataTree::expectOne()
        (RETURN)
            (BOOLEAN, "false")
        (END_OF_STATEMENT)
        (RETURN_NOTHING)
    ;
}

TEST_F(StmtNodesTest, Block)
{
    inst::Block b0;
    b0.addStmt(util::mkptr(new inst::Initialization(1
                                                  , 0
                                                  , util::mkptr(new inst::IntLiteral(2128))
                                                  , util::mkptr(new inst::IntPrimitive))));
    b0.addStmt(util::mkptr(new inst::ReturnNothing));

    b0.write();

    DataTree::expectOne()
        (BLOCK_BEGIN)
            (INITIALIZE_THIS_LEVEL, "int", 0)
                (EXPRESSION_BEGIN)
                (INTEGER, "2128")
                (EXPRESSION_END)
            (END_OF_STATEMENT)
            (RETURN_NOTHING)
        (BLOCK_END)
    ;
}
