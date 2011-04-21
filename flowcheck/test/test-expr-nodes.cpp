#include <vector>
#include <gtest/gtest.h>

#include "test-common.h"
#include "../expr-nodes.h"
#include "../../proto/node-base.h"
#include "../../proto/function.h"
#include "../../proto/global-scope.h"
#include "../../instance/node-base.h"
#include "../../test/phony-errors.h"
#include "../../test/common.h"

using namespace test;

typedef flowcheck_test ExprNodesTest;

TEST_F(ExprNodesTest, Literals)
{
    misc::position pos(1);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));
    flchk::IntLiteral int0(pos, "20110116");
    int0.compile(*scope)->inst(nul_inst_scope);
    EXPECT_TRUE(int0.is_literal());

    flchk::FloatLiteral float0(pos, "19.50");
    float0.compile(*scope)->inst(nul_inst_scope);
    EXPECT_TRUE(float0.is_literal());

    flchk::BoolLiteral bool0(pos, true);
    bool0.compile(*scope)->inst(nul_inst_scope);
    EXPECT_TRUE(bool0.is_literal());
    EXPECT_TRUE(bool0.bool_value());

    flchk::IntLiteral int1(pos, "441499");
    int1.compile(*scope)->inst(nul_inst_scope);
    EXPECT_TRUE(int1.is_literal());

    flchk::FloatLiteral float1(pos, "0.1950");
    float1.compile(*scope)->inst(nul_inst_scope);
    EXPECT_TRUE(float1.is_literal());

    flchk::BoolLiteral bool1(pos, false);
    bool1.compile(*scope)->inst(nul_inst_scope);
    EXPECT_TRUE(bool1.is_literal());
    EXPECT_FALSE(bool1.bool_value());

    EXPECT_FALSE(error::has_error());

    data_tree::expect_one()
        (pos, INTEGER, "20110116")
        (pos, FLOATING, "19.5")
        (pos, BOOLEAN, "true")
        (pos, INTEGER, "441499")
        (pos, FLOATING, "0.195")
        (pos, BOOLEAN, "false")
    ;
}

TEST_F(ExprNodesTest, Reference)
{
    misc::position pos(1);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));
    flchk::reference ref0(pos, "a20110116");
    EXPECT_FALSE(ref0.is_literal());
    ref0.compile(*scope)->inst(nul_inst_scope);

    flchk::reference ref1(pos, "b1950");
    EXPECT_FALSE(ref0.is_literal());
    ref1.compile(*scope)->inst(nul_inst_scope);

    EXPECT_FALSE(error::has_error());

    data_tree::expect_one()
        (pos, REFERENCE, "a20110116")
        (pos, REFERENCE, "b1950")
    ;
}

TEST_F(ExprNodesTest, Operations)
{
    misc::position pos(2);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));
    flchk::binary_op binary0(pos
                           , std::move(util::mkptr(new flchk::IntLiteral(pos, "1")))
                           , "+"
                           , std::move(util::mkptr(new flchk::FloatLiteral(pos, "11235.8"))));
    flchk::binary_op binary1(pos
                           , std::move(util::mkptr(new flchk::FloatLiteral(pos, "1.12358")))
                           , "<="
                           , std::move(util::mkptr(new flchk::IntLiteral(pos, "2357111317"))));

    flchk::pre_unary_op pre_unary0(pos, "+", std::move(util::mkptr(new flchk::FloatLiteral(pos, ".13"))));
    flchk::pre_unary_op pre_unary1(
            pos
          , "-"
          , std::move(util::mkptr(
                    new flchk::binary_op(pos
                                       , std::move(util::mkptr(new flchk::reference(pos, "wasureru")))
                                       , "%"
                                       , std::move(util::mkptr(new flchk::IntLiteral(pos, "1")))))));

    flchk::conjunction conj(pos
                          , std::move(util::mkptr(new flchk::BoolLiteral(pos, true)))
                          , std::move(util::mkptr(new flchk::reference(pos, "koto"))));
    flchk::disjunction disj(pos
                          , std::move(util::mkptr(new flchk::BoolLiteral(pos, false)))
                          , std::move(util::mkptr(new flchk::IntLiteral(pos, "2"))));
    flchk::negation nega(pos, std::move(util::mkptr(new flchk::FloatLiteral(pos, "1954.01"))));

    binary0.compile(*scope)->inst(nul_inst_scope);
    EXPECT_TRUE(binary0.is_literal());

    binary1.compile(*scope)->inst(nul_inst_scope);
    EXPECT_TRUE(binary1.is_literal());
    EXPECT_TRUE(binary1.bool_value());

    pre_unary0.compile(*scope)->inst(nul_inst_scope);
    EXPECT_TRUE(pre_unary0.is_literal());

    pre_unary1.compile(*scope)->inst(nul_inst_scope);
    EXPECT_FALSE(pre_unary1.is_literal());

    conj.compile(*scope)->inst(nul_inst_scope);
    EXPECT_FALSE(conj.is_literal());

    disj.compile(*scope)->inst(nul_inst_scope);
    EXPECT_TRUE(disj.is_literal());

    nega.compile(*scope)->inst(nul_inst_scope);
    EXPECT_TRUE(nega.is_literal());

    EXPECT_FALSE(error::has_error());

    data_tree::expect_one()
        (pos, BINARY_OP, "+")
            (pos, INTEGER, "1")
            (pos, FLOATING, "11235.8")
        (pos, BINARY_OP, "<=")
            (pos, FLOATING, "1.12358")
            (pos, INTEGER, "2357111317")
        (pos, PRE_UNARY_OP, "+")
            (pos, FLOATING, "0.13")
        (pos, PRE_UNARY_OP, "-")
            (pos, BINARY_OP, "%")
                (pos, REFERENCE, "wasureru")
                (pos, INTEGER, "1")
        (pos, BINARY_OP, "&&")
            (pos, BOOLEAN, "true")
            (pos, REFERENCE, "koto")
        (pos, BINARY_OP, "||")
            (pos, BOOLEAN, "false")
            (pos, INTEGER, "2")
        (pos, PRE_UNARY_OP, "!")
            (pos, FLOATING, "1954.01")
    ;
}

TEST_F(ExprNodesTest, Calls)
{
    misc::position pos(3);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));

    std::vector<util::sptr<flchk::Expression const>> params;
    flchk::call call0(pos, "fib", std::move(params));

    params.push_back(std::move(util::mkptr(new flchk::BoolLiteral(pos, false))));
    params.push_back(std::move(util::mkptr(
                    new flchk::pre_unary_op(pos, "-", std::move(util::mkptr(
                                new flchk::FloatLiteral(pos, "11.11")))))));
    params.push_back(std::move(util::mkptr(
                    new flchk::negation(pos, std::move(util::mkptr(
                                new flchk::IntLiteral(pos, "21")))))));
    params.push_back(std::move(util::mkptr(new flchk::reference(pos, "dareka_tasukete_kudasai"))));
    flchk::call call1(pos, "leap", std::move(params));

    call0.compile(*scope)->inst(nul_inst_scope);
    EXPECT_FALSE(call0.is_literal());

    call1.compile(*scope)->inst(nul_inst_scope);
    EXPECT_FALSE(call1.is_literal());

    EXPECT_FALSE(error::has_error());

    data_tree::expect_one()
        (pos, CALL, "fib", 0, false)
        (pos, CALL, "leap", 4, false)
            (pos, BOOLEAN, "false")
            (pos, PRE_UNARY_OP, "-")
                (pos, FLOATING, "11.11")
            (pos, PRE_UNARY_OP, "!")
                (pos, INTEGER, "21")
            (pos, REFERENCE, "dareka_tasukete_kudasai")
    ;
}

TEST_F(ExprNodesTest, FuncReference)
{
    misc::position pos(4);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));

    flchk::func_reference func_ref0(pos, "fib", 2);
    flchk::func_reference func_ref1(pos, "fib", 1);
    flchk::func_reference func_ref2(pos, "fib", 2);

    func_ref0.compile(*scope)->inst(nul_inst_scope);
    EXPECT_FALSE(func_ref0.is_literal());

    func_ref1.compile(*scope)->inst(nul_inst_scope);
    EXPECT_FALSE(func_ref0.is_literal());

    func_ref2.compile(*scope)->inst(nul_inst_scope);
    EXPECT_FALSE(func_ref0.is_literal());

    EXPECT_FALSE(error::has_error());

    data_tree::expect_one()
        (pos, FUNC_REFERENCE, "fib", 2, false)
        (pos, FUNC_REFERENCE, "fib", 1, false)
        (pos, FUNC_REFERENCE, "fib", 2, false)
    ;
}

TEST_F(ExprNodesTest, LiteralBoolValueError)
{
    misc::position pos(5);
    flchk::IntLiteral int0(pos, "20110409");
    int0.bool_value();

    flchk::FloatLiteral float0(pos, "10.58");
    float0.bool_value();

    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(2, get_cond_not_bools().size());
    EXPECT_EQ(pos, get_cond_not_bools()[0].pos);
    EXPECT_EQ("(int(20110409))", get_cond_not_bools()[0].type_name);
    EXPECT_EQ(pos, get_cond_not_bools()[1].pos);
    EXPECT_EQ("(float(10.58))", get_cond_not_bools()[1].type_name);
}

TEST_F(ExprNodesTest, OperationLiteralBoolValueError)
{
    misc::position pos(6);
    flchk::conjunction conj(pos
                          , std::move(util::mkptr(new flchk::BoolLiteral(pos, true)))
                          , std::move(util::mkptr(new flchk::FloatLiteral(pos, "20110.4"))));
    conj.bool_value();
    flchk::disjunction disj(pos
                          , std::move(util::mkptr(new flchk::BoolLiteral(pos, false)))
                          , std::move(util::mkptr(new flchk::IntLiteral(pos, "2"))));
    disj.bool_value();
    flchk::negation nega(pos, std::move(util::mkptr(new flchk::FloatLiteral(pos, "1.12"))));
    nega.bool_value();

    flchk::binary_op binary(pos
                          , std::move(util::mkptr(new flchk::IntLiteral(pos, "1")))
                          , "*"
                          , std::move(util::mkptr(new flchk::FloatLiteral(pos, "11235.8"))));
    binary.bool_value();

    flchk::pre_unary_op pre_unary(pos, "+", std::move(util::mkptr(new flchk::FloatLiteral(pos, ".13"))));
    pre_unary.bool_value();

    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(5, get_cond_not_bools().size());
    EXPECT_EQ(pos, get_cond_not_bools()[0].pos);
    EXPECT_EQ("(float(20110.4))", get_cond_not_bools()[0].type_name);
    EXPECT_EQ(pos, get_cond_not_bools()[1].pos);
    EXPECT_EQ("(int(2))", get_cond_not_bools()[1].type_name);
    EXPECT_EQ(pos, get_cond_not_bools()[2].pos);
    EXPECT_EQ("(float(1.12))", get_cond_not_bools()[2].type_name);
    EXPECT_EQ(pos, get_cond_not_bools()[3].pos);
    EXPECT_EQ("((int(1))*(float(11235.8)))", get_cond_not_bools()[3].type_name);
    EXPECT_EQ(pos, get_cond_not_bools()[4].pos);
    EXPECT_EQ("(+(float(0.13)))", get_cond_not_bools()[4].type_name);
}
