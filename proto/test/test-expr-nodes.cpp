#include <gtest/gtest.h>

#include "test-common.h"
#include "../expr-nodes.h"
#include "../../test/phony-errors.h"

using namespace test;

struct ExprNodesTest
    : public proto_test
{
    void SetUp()
    {
        proto_test::SetUp();
        inst_scope.reset(new phony_func);
    }

    util::sptr<inst::Scope> inst_scope;
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

    bool0.inst(*inst_scope)->typeof();
    bool1.inst(*inst_scope)->typeof();
    int0.inst(*inst_scope)->typeof();
    int1.inst(*inst_scope)->typeof();
    float0.inst(*inst_scope)->typeof();
    float1.inst(*inst_scope)->typeof();
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
    proto::Reference ref0(pos, "ushiro");
    proto::Reference ref1(pos, "moji");
    ref0.inst(*inst_scope)->typeof();
    ref1.inst(*inst_scope)->typeof();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (pos, QUERY_VAR, "ushiro")
        (REFERENCE)
        (pos, QUERY_VAR, "moji")
        (REFERENCE)
    ;
}

TEST_F(ExprNodesTest, Operations)
{
    misc::position pos(3);
    proto::BinaryOp bin(pos
                       , std::move(util::mkptr(new proto::IntLiteral(pos, 20110122)))
                       , "+"
                       , std::move(util::mkptr(new proto::Reference(pos, "littleBird"))));
    proto::PreUnaryOp preu(pos
                           , "-"
                           , std::move(util::mkptr(new proto::Reference(pos, "uninstall"))));

    bin.inst(*inst_scope)->typeof();
    preu.inst(*inst_scope)->typeof();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
            (pos, QUERY_VAR, "littleBird")
            (INTEGER, "20110122")
            (REFERENCE)
        (pos, QUERY_BINARY_OP, "+")

        (BINARY_OP)
            (INTEGER, "20110122")
            (REFERENCE)

            (pos, QUERY_VAR, "uninstall")
            (REFERENCE)
        (pos, QUERY_PRE_UNARY_OP, "-")

        (PRE_UNARY_OP)
            (REFERENCE)
    ;
}

TEST_F(ExprNodesTest, Logic)
{
    misc::position pos(4);
    proto::Conjunction conj(pos
                          , std::move(util::mkptr(new proto::BoolLiteral(pos, true)))
                          , std::move(util::mkptr(new proto::BoolLiteral(pos, false))));
    proto::Disjunction disj(pos
                          , std::move(util::mkptr(new proto::IntLiteral(pos, 198765432)))
                          , std::move(util::mkptr(new proto::IntLiteral(pos, 0))));
    proto::Negation nega(pos, std::move(util::mkptr(new proto::BoolLiteral(pos, true))));

    conj.inst(*inst_scope)->typeof();
    disj.inst(*inst_scope)->typeof();
    nega.inst(*inst_scope)->typeof();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (CONJUNCTION)
            (BOOLEAN, "true")
            (BOOLEAN, "false")

        (DISJUNCTION)
            (INTEGER, "198765432")
            (INTEGER, "0")

        (NEGATION)
            (BOOLEAN, "true")
    ;
}
