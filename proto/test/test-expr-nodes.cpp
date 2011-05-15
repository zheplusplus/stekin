#include <gtest/gtest.h>

#include "test-common.h"
#include "../expr-nodes.h"
#include "../symbol-table.h"
#include "../variable.h"
#include "../../test/phony-errors.h"

using namespace test;

struct ExprNodesTest
    : public ProtoTest
{
    void SetUp()
    {
        ProtoTest::SetUp();
        global_st.reset(new proto::SymbolTable);
    }

    util::sptr<proto::SymbolTable> global_st;
};

TEST_F(ExprNodesTest, Literals)
{
    misc::position pos(1);
    proto::BoolLiteral bool0(pos, true);
    proto::BoolLiteral bool1(pos, false);
    proto::IntLiteral int0(pos, 0);
    proto::IntLiteral int1(pos, 1048576);
    proto::FloatLiteral float0(pos, 0.4321);
    proto::FloatLiteral float1(pos, 1234.56);

    bool0.inst(*global_st)->write();
    bool1.inst(*global_st)->write();
    int0.inst(*global_st)->write();
    int1.inst(*global_st)->write();
    float0.inst(*global_st)->write();
    float1.inst(*global_st)->write();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (BOOLEAN, "true")
        (BOOLEAN, "false")
        (INTEGER, "0")
        (INTEGER, "1048576")
        (FLOATING, "0.4321")
        (FLOATING, "1234.56")
    ;
}

TEST_F(ExprNodesTest, Reference)
{
    misc::position pos(2);
    global_st->defVar(pos, proto::Type::BIT_BOOL, "ushiro");
    global_st->defVar(pos, proto::Type::BIT_INT, "moji");
    ASSERT_FALSE(error::hasError());

    proto::Reference ref0(pos, "ushiro");
    proto::Reference ref1(pos, "moji");
    ref0.inst(*global_st)->write();
    ref1.inst(*global_st)->write();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (REFERENCE)
        (REFERENCE)
    ;
}

TEST_F(ExprNodesTest, Operations)
{
    misc::position pos(3);
    global_st->defVar(pos, proto::Type::BIT_INT, "littleBird");
    global_st->defVar(pos, proto::Type::BIT_FLOAT, "uninstall");
    ASSERT_FALSE(error::hasError());

    proto::BinaryOp bin(pos
                      , util::mkptr(new proto::IntLiteral(pos, mpz_class("20110122")))
                      , "+"
                      , util::mkptr(new proto::Reference(pos, "littleBird")));
    proto::PreUnaryOp preu(pos, "-", util::mkptr(new proto::Reference(pos, "uninstall")));

    bin.inst(*global_st)->write();
    preu.inst(*global_st)->write();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (BINARY_OP, "+")
            (INTEGER, "20110122")
            (REFERENCE)
        (PRE_UNARY_OP, "-")
            (REFERENCE)
    ;
}

TEST_F(ExprNodesTest, Logic)
{
    misc::position pos(4);
    global_st->defVar(pos, proto::Type::BIT_BOOL, "vermillion");
    proto::Conjunction conj(pos
                          , util::mkptr(new proto::BoolLiteral(pos, true))
                          , util::mkptr(new proto::Reference(pos, "vermillion")));
    proto::Disjunction disj(pos
                          , util::mkptr(new proto::Reference(pos, "vermillion"))
                          , util::mkptr(new proto::BoolLiteral(pos, false)));
    proto::Negation nega(pos, util::mkptr(new proto::Reference(pos, "vermillion")));

    conj.inst(*global_st)->write();
    disj.inst(*global_st)->write();
    nega.inst(*global_st)->write();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (CONJUNCTION)
            (BOOLEAN, "true")
            (REFERENCE)
        (DISJUNCTION)
            (REFERENCE)
            (BOOLEAN, "false")
        (NEGATION)
            (REFERENCE)
    ;
}

TEST_F(ExprNodesTest, ConjunctionConditionTypeCheck)
{
    misc::position pos(5);
    global_st->defVar(pos, proto::Type::BIT_INT, "chizu");
    proto::Conjunction(pos
                     , util::mkptr(new proto::BoolLiteral(pos, true))
                     , util::mkptr(new proto::Reference(pos, "chizu")))
        .inst(*global_st);
    proto::Conjunction(pos
                     , util::mkptr(new proto::Reference(pos, "chizu"))
                     , util::mkptr(new proto::BoolLiteral(pos, false)))
        .inst(*global_st);

    EXPECT_TRUE(error::hasError());
    ASSERT_EQ(2, getCondNotBools().size());
    ASSERT_EQ(pos, getCondNotBools()[0].pos);
    ASSERT_EQ(proto::Type::BIT_INT->name(), getCondNotBools()[0].type_name);
    ASSERT_EQ(pos, getCondNotBools()[1].pos);
    ASSERT_EQ(proto::Type::BIT_INT->name(), getCondNotBools()[1].type_name);
}

TEST_F(ExprNodesTest, DisjunctionConditionTypeCheck)
{
    misc::position pos(6);
    global_st->defVar(pos, proto::Type::BIT_FLOAT, "kako");
    proto::Disjunction(pos
                     , util::mkptr(new proto::Reference(pos, "kako"))
                     , util::mkptr(new proto::BoolLiteral(pos, true)))
        .inst(*global_st);
    proto::Disjunction(pos
                     , util::mkptr(new proto::BoolLiteral(pos, false))
                     , util::mkptr(new proto::Reference(pos, "kako")))
        .inst(*global_st);

    EXPECT_TRUE(error::hasError());
    ASSERT_EQ(2, getCondNotBools().size());
    ASSERT_EQ(pos, getCondNotBools()[0].pos);
    ASSERT_EQ(proto::Type::BIT_FLOAT->name(), getCondNotBools()[0].type_name);
    ASSERT_EQ(pos, getCondNotBools()[1].pos);
    ASSERT_EQ(proto::Type::BIT_FLOAT->name(), getCondNotBools()[1].type_name);
}

TEST_F(ExprNodesTest, NegationConditionTypeCheck)
{
    misc::position pos(7);
    global_st->defVar(pos, proto::Type::BIT_VOID, "kirie");
    proto::Negation(pos, util::mkptr(new proto::Reference(pos, "kirie"))).inst(*global_st);

    EXPECT_TRUE(error::hasError());
    ASSERT_EQ(1, getCondNotBools().size());
    ASSERT_EQ(pos, getCondNotBools()[0].pos);
    ASSERT_EQ(proto::Type::BIT_VOID->name(), getCondNotBools()[0].type_name);
}
