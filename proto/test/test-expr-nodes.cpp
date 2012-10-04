#include <gtest/gtest.h>

#include <test/phony-errors.h>

#include "test-common.h"
#include "../expr-nodes.h"
#include "../list-types.h"
#include "../symbol-table.h"
#include "../variable.h"

using namespace test;

struct ExprNodesTest
    : public ProtoTest
{
    ExprNodesTest()
        : global_st(nullptr)
    {}

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
    misc::trace trace;
    trace.add(pos);
    proto::BoolLiteral bool0(pos, true);
    proto::BoolLiteral bool1(pos, false);
    proto::IntLiteral int0(pos, 0);
    proto::IntLiteral int1(pos, 1048576);
    proto::FloatLiteral float0(pos, 0.4321);
    proto::FloatLiteral float1(pos, 1234.56);
    proto::StringLiteral str(pos, "eternal rite");

    bool0.inst(*global_st, trace)->write();
    bool1.inst(*global_st, trace)->write();
    int0.inst(*global_st, trace)->write();
    int1.inst(*global_st, trace)->write();
    float0.inst(*global_st, trace)->write();
    float1.inst(*global_st, trace)->write();
    str.inst(*global_st, trace)->write();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (BOOLEAN, "true")
        (BOOLEAN, "false")
        (INTEGER, "0")
        (INTEGER, "1048576")
        (FLOATING, "0.4321")
        (FLOATING, "1234.56")
        (STRING, "eternal rite")
    ;
}

TEST_F(ExprNodesTest, Reference)
{
    misc::position pos(2);
    misc::trace trace;
    trace.add(pos);
    global_st->defVar(pos, proto::Type::s_bool(), "ushiro");
    global_st->defVar(pos, proto::Type::s_int(), "moji");
    ASSERT_FALSE(error::hasError());

    proto::Reference ref0(pos, "ushiro");
    proto::Reference ref1(pos, "moji");
    ref0.inst(*global_st, trace)->write();
    ref1.inst(*global_st, trace)->write();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (REFERENCE)
        (REFERENCE)
    ;
}

TEST_F(ExprNodesTest, StringOperations)
{
    misc::position pos(12);
    misc::trace trace;
    trace.add(pos);
    global_st->defVar(pos, proto::Type::s_string(), "yoi_kaze_uta");
    ASSERT_FALSE(error::hasError());

    proto::BinaryOp bin(pos
                      , util::mkptr(new proto::IntLiteral(pos, mpz_class("20110122")))
                      , "+"
                      , util::mkptr(new proto::Reference(pos, "yoi_kaze_uta")));

    bin.inst(*global_st, trace)->write();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (BINARY_OP, "+")
            (INTEGER, "20110122")
            (REFERENCE)
    ;
}

TEST_F(ExprNodesTest, NumericOperations)
{
    misc::position pos(3);
    misc::trace trace;
    trace.add(pos);
    global_st->defVar(pos, proto::Type::s_int(), "littleBird");
    global_st->defVar(pos, proto::Type::s_float(), "uninstall");
    ASSERT_FALSE(error::hasError());

    proto::BinaryOp bin(pos
                      , util::mkptr(new proto::IntLiteral(pos, mpz_class("20110122")))
                      , "+"
                      , util::mkptr(new proto::Reference(pos, "littleBird")));
    proto::PreUnaryOp preu(pos, "-", util::mkptr(new proto::Reference(pos, "uninstall")));

    bin.inst(*global_st, trace)->write();
    preu.inst(*global_st, trace)->write();
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
    misc::trace trace;
    trace.add(pos);
    global_st->defVar(pos, proto::Type::s_bool(), "vermillion");
    proto::Conjunction conj(pos
                          , util::mkptr(new proto::BoolLiteral(pos, true))
                          , util::mkptr(new proto::Reference(pos, "vermillion")));
    proto::Disjunction disj(pos
                          , util::mkptr(new proto::Reference(pos, "vermillion"))
                          , util::mkptr(new proto::BoolLiteral(pos, false)));
    proto::Negation nega(pos, util::mkptr(new proto::Reference(pos, "vermillion")));

    conj.inst(*global_st, trace)->write();
    disj.inst(*global_st, trace)->write();
    nega.inst(*global_st, trace)->write();
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
    misc::trace trace;
    trace.add(pos);
    global_st->defVar(pos, proto::Type::s_int(), "chizu");
    proto::Conjunction(pos
                     , util::mkptr(new proto::BoolLiteral(pos, true))
                     , util::mkptr(new proto::Reference(pos, "chizu")))
        .inst(*global_st, trace);
    proto::Conjunction(pos
                     , util::mkptr(new proto::Reference(pos, "chizu"))
                     , util::mkptr(new proto::BoolLiteral(pos, false)))
        .inst(*global_st, trace);

    EXPECT_TRUE(error::hasError());
    ASSERT_EQ(2, getCondNotBools().size());
    ASSERT_EQ(pos, getCondNotBools()[0].pos);
    ASSERT_EQ(proto::Type::s_int()->name(), getCondNotBools()[0].type_name);
    ASSERT_EQ(pos, getCondNotBools()[1].pos);
    ASSERT_EQ(proto::Type::s_int()->name(), getCondNotBools()[1].type_name);
}

TEST_F(ExprNodesTest, DisjunctionConditionTypeCheck)
{
    misc::position pos(6);
    misc::trace trace;
    trace.add(pos);
    global_st->defVar(pos, proto::Type::s_float(), "kako");
    proto::Disjunction(pos
                     , util::mkptr(new proto::Reference(pos, "kako"))
                     , util::mkptr(new proto::BoolLiteral(pos, true)))
        .inst(*global_st, trace);
    proto::Disjunction(pos
                     , util::mkptr(new proto::BoolLiteral(pos, false))
                     , util::mkptr(new proto::Reference(pos, "kako")))
        .inst(*global_st, trace);

    EXPECT_TRUE(error::hasError());
    ASSERT_EQ(2, getCondNotBools().size());
    ASSERT_EQ(pos, getCondNotBools()[0].pos);
    ASSERT_EQ(proto::Type::s_float()->name(), getCondNotBools()[0].type_name);
    ASSERT_EQ(pos, getCondNotBools()[1].pos);
    ASSERT_EQ(proto::Type::s_float()->name(), getCondNotBools()[1].type_name);
}

TEST_F(ExprNodesTest, NegationConditionTypeCheck)
{
    misc::position pos(7);
    misc::trace trace;
    trace.add(pos);
    global_st->defVar(pos, proto::Type::s_void(), "kirie");
    proto::Negation(pos, util::mkptr(new proto::Reference(pos, "kirie"))).inst(*global_st, trace);

    EXPECT_TRUE(error::hasError());
    ASSERT_EQ(1, getCondNotBools().size());
    ASSERT_EQ(pos, getCondNotBools()[0].pos);
    ASSERT_EQ(proto::Type::s_void()->name(), getCondNotBools()[0].type_name);
}

TEST_F(ExprNodesTest, ListLiterals)
{
    misc::position pos(8);
    misc::position err_pos(800);
    misc::trace trace;
    trace.add(err_pos);
    global_st->defVar(pos, proto::Type::s_int(), "sekine");
    global_st->defVar(pos, proto::Type::s_int(), "yui");
    global_st->defVar(pos, proto::Type::s_bool(), "shina");

    std::vector<util::sptr<proto::Expression const>> members;

    members.push_back(util::mkptr(new proto::Reference(pos, "sekine")));
    members.push_back(util::mkptr(new proto::Reference(pos, "yui")));
    proto::ListLiteral ls_a(pos, std::move(members));
    ls_a.inst(*global_st, trace)->write();
    EXPECT_FALSE(error::hasError());
    EXPECT_EQ(ls_a.type(*global_st, trace), proto::ListType::getListType(proto::Type::s_int()));

    members.push_back(util::mkptr(new proto::Reference(pos, "shina")));
    proto::ListLiteral ls_b(pos, std::move(members));
    ls_b.inst(*global_st, trace)->write();
    EXPECT_FALSE(error::hasError());
    EXPECT_EQ(ls_b.type(*global_st, trace), proto::ListType::getListType(proto::Type::s_bool()));

    members.push_back(util::mkptr(new proto::Reference(err_pos, "sekine")));
    members.push_back(util::mkptr(new proto::Reference(err_pos, "shina")));
    misc::trace err_trace;
    proto::ListLiteral(err_pos, std::move(members)).inst(*global_st, err_trace);
    EXPECT_TRUE(error::hasError());

    ASSERT_EQ(1, getListMemberTypesNotSame().size());
    ASSERT_EQ(err_pos, getListMemberTypesNotSame()[0].pos);

    DataTree::expectOne()
        (LIST_BEGIN)
            (REFERENCE)
            (REFERENCE)
        (LIST_END)
        (LIST_BEGIN)
            (REFERENCE)
        (LIST_END)
    ;
}

TEST_F(ExprNodesTest, EmptyListLiterals)
{
    misc::position pos(9);
    misc::trace trace;

    proto::ListLiteral ls(pos, std::vector<util::sptr<proto::Expression const>>());
    ls.inst(*global_st, trace)->write();
    EXPECT_FALSE(error::hasError());
    EXPECT_EQ(ls.type(*global_st, trace), proto::ListType::getEmptyListType());

    DataTree::expectOne()
        (EMPTY_LIST)
    ;
}

TEST_F(ExprNodesTest, ListAppending)
{
    misc::position pos(10);
    misc::trace trace;

    global_st->defVar(pos, proto::ListType::getListType(proto::Type::s_int()), "chiaki");

    proto::ListAppend la(pos
                       , util::mkptr(new proto::Reference(pos, "chiaki"))
                       , util::mkptr(new proto::ListLiteral(
                                        pos, std::vector<util::sptr<proto::Expression const>>())));
    la.inst(*global_st, trace)->write();
    EXPECT_EQ(la.type(*global_st, trace), proto::ListType::getListType(proto::Type::s_int()));
    EXPECT_FALSE(error::hasError());

    DataTree::expectOne()
        (BINARY_OP, "++")
            (REFERENCE)
            (EMPTY_LIST)
    ;
}

TEST_F(ExprNodesTest, ListAppendingError)
{
    misc::position pos(11);
    misc::trace trace;

    proto::ListAppend la(pos
                       , util::mkptr(new proto::IntLiteral(pos, 20110823))
                       , util::mkptr(new proto::ListLiteral(
                                        pos, std::vector<util::sptr<proto::Expression const>>())));
    la.type(*global_st, trace);
    EXPECT_TRUE(error::hasError());

    ASSERT_EQ(1, getNABinaryOps().size());
    EXPECT_EQ(pos, getNABinaryOps()[0].pos);
    EXPECT_EQ("++", getNABinaryOps()[0].op_img);
    EXPECT_EQ("int", getNABinaryOps()[0].lhst_name);
    EXPECT_EQ("Stekin empty list", getNABinaryOps()[0].rhst_name);
}
