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

    util::sptr<inst::scope> inst_scope;
};

TEST_F(ExprNodesTest, Literals)
{
    misc::pos_type pos(1);
    proto::bool_literal bool0(pos, true);
    proto::bool_literal bool1(pos, false);
    proto::IntLiteral int0(pos, 0);
    proto::IntLiteral int1(pos, 1048576);
    proto::float_literal float0(pos, 0.4321);
    proto::float_literal float1(pos, 1234.56);

    bool0.inst(*inst_scope)->typeof();
    bool1.inst(*inst_scope)->typeof();
    int0.inst(*inst_scope)->typeof();
    int1.inst(*inst_scope)->typeof();
    float0.inst(*inst_scope)->typeof();
    float1.inst(*inst_scope)->typeof();
    ASSERT_FALSE(error::has_error());

    data_tree::expect_one()
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
    misc::pos_type pos(2);
    proto::reference ref0(pos, "ushiro");
    proto::reference ref1(pos, "moji");
    ref0.inst(*inst_scope)->typeof();
    ref1.inst(*inst_scope)->typeof();
    ASSERT_FALSE(error::has_error());

    data_tree::expect_one()
        (pos, QUERY_VAR, "ushiro")
        (REFERENCE)
        (pos, QUERY_VAR, "moji")
        (REFERENCE)
    ;
}

TEST_F(ExprNodesTest, Operations)
{
    misc::pos_type pos(3);
    proto::binary_op bin(pos
                       , std::move(util::mkptr(new proto::IntLiteral(pos, 20110122)))
                       , "+"
                       , std::move(util::mkptr(new proto::reference(pos, "littleBird"))));
    proto::pre_unary_op preu(pos
                           , "-"
                           , std::move(util::mkptr(new proto::reference(pos, "uninstall"))));

    bin.inst(*inst_scope)->typeof();
    preu.inst(*inst_scope)->typeof();
    ASSERT_FALSE(error::has_error());

    data_tree::expect_one()
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
    misc::pos_type pos(4);
    proto::conjunction conj(pos
                          , std::move(util::mkptr(new proto::bool_literal(pos, true)))
                          , std::move(util::mkptr(new proto::bool_literal(pos, false))));
    proto::disjunction disj(pos
                          , std::move(util::mkptr(new proto::IntLiteral(pos, 198765432)))
                          , std::move(util::mkptr(new proto::IntLiteral(pos, 0))));
    proto::negation nega(pos, std::move(util::mkptr(new proto::bool_literal(pos, true))));

    conj.inst(*inst_scope)->typeof();
    disj.inst(*inst_scope)->typeof();
    nega.inst(*inst_scope)->typeof();
    ASSERT_FALSE(error::has_error());

    data_tree::expect_one()
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
