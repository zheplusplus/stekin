#include <vector>
#include <gtest/gtest.h>

#include "test-common.h"
#include "../expr-nodes.h"
#include "../global-filter.h"
#include "../func-body-filter.h"
#include "../function.h"
#include "../symbol-table.h"
#include "../../proto/node-base.h"
#include "../../proto/function.h"
#include "../../instance/node-base.h"
#include "../../test/phony-errors.h"
#include "../../test/common.h"

using namespace test;

typedef FlowcheckTest ExprNodesTest;

TEST_F(ExprNodesTest, Literals)
{
    misc::position pos(1);
    util::sptr<proto::Block> block(new proto::Block);
    flchk::SymbolTable st;
    flchk::IntLiteral int0(pos, "20110116");
    int0.compile(*block, util::mkref(st))->inst(nul_st);
    EXPECT_TRUE(int0.isLiteral());

    flchk::FloatLiteral float0(pos, "19.50");
    float0.compile(*block, util::mkref(st))->inst(nul_st);
    EXPECT_TRUE(float0.isLiteral());

    flchk::BoolLiteral bool0(pos, true);
    bool0.compile(*block, util::mkref(st))->inst(nul_st);
    EXPECT_TRUE(bool0.isLiteral());
    EXPECT_TRUE(bool0.boolValue());

    flchk::IntLiteral int1(pos, "441499");
    int1.compile(*block, util::mkref(st))->inst(nul_st);
    EXPECT_TRUE(int1.isLiteral());

    flchk::FloatLiteral float1(pos, "0.1950");
    float1.compile(*block, util::mkref(st))->inst(nul_st);
    EXPECT_TRUE(float1.isLiteral());

    flchk::BoolLiteral bool1(pos, false);
    bool1.compile(*block, util::mkref(st))->inst(nul_st);
    EXPECT_TRUE(bool1.isLiteral());
    EXPECT_FALSE(bool1.boolValue());

    EXPECT_FALSE(error::hasError());

    DataTree::expectOne()
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
    util::sptr<proto::Block> block(new proto::Block);
    flchk::GlobalFilter filter;
    flchk::Reference ref0(pos, "a20110116");
    EXPECT_FALSE(ref0.isLiteral());
    ref0.compile(*block, filter.getSymbols())->inst(nul_st);

    flchk::Reference ref1(pos, "b1950");
    EXPECT_FALSE(ref0.isLiteral());
    ref1.compile(*block, filter.getSymbols())->inst(nul_st);

    EXPECT_FALSE(error::hasError());

    DataTree::expectOne()
        (pos, REFERENCE, "a20110116")
        (pos, REFERENCE, "b1950")
    ;
}

TEST_F(ExprNodesTest, Operations)
{
    misc::position pos(2);
    util::sptr<proto::Block> block(new proto::Block);
    flchk::GlobalFilter filter;
    flchk::BinaryOp binary0(pos
                          , std::move(util::mkptr(new flchk::IntLiteral(pos, "1")))
                          , "+"
                          , std::move(util::mkptr(new flchk::FloatLiteral(pos, "11235.8"))));
    flchk::BinaryOp binary1(pos
                          , std::move(util::mkptr(new flchk::FloatLiteral(pos, "1.12358")))
                          , "<="
                          , std::move(util::mkptr(new flchk::IntLiteral(pos, "2357111317"))));

    flchk::PreUnaryOp pre_unary0(pos, "+", util::mkptr(new flchk::FloatLiteral(pos, ".13")));
    flchk::PreUnaryOp pre_unary1(
            pos
          , "-"
          , util::mkptr(new flchk::BinaryOp(pos
                                          , util::mkptr(new flchk::Reference(pos, "wasureru"))
                                          , "%"
                                          , util::mkptr(new flchk::IntLiteral(pos, "1")))));

    flchk::Conjunction conj(pos
                          , util::mkptr(new flchk::BoolLiteral(pos, true))
                          , util::mkptr(new flchk::Reference(pos, "koto")));
    flchk::Disjunction disj(pos
                          , util::mkptr(new flchk::BoolLiteral(pos, false))
                          , util::mkptr(new flchk::IntLiteral(pos, "2")));
    flchk::Negation nega(pos, util::mkptr(new flchk::FloatLiteral(pos, "1954.01")));

    binary0.compile(*block, filter.getSymbols())->inst(nul_st);
    EXPECT_TRUE(binary0.isLiteral());

    binary1.compile(*block, filter.getSymbols())->inst(nul_st);
    EXPECT_TRUE(binary1.isLiteral());
    EXPECT_TRUE(binary1.boolValue());

    pre_unary0.compile(*block, filter.getSymbols())->inst(nul_st);
    EXPECT_TRUE(pre_unary0.isLiteral());

    pre_unary1.compile(*block, filter.getSymbols())->inst(nul_st);
    EXPECT_FALSE(pre_unary1.isLiteral());

    conj.compile(*block, filter.getSymbols())->inst(nul_st);
    EXPECT_FALSE(conj.isLiteral());

    disj.compile(*block, filter.getSymbols())->inst(nul_st);
    EXPECT_TRUE(disj.isLiteral());

    nega.compile(*block, filter.getSymbols())->inst(nul_st);
    EXPECT_TRUE(nega.isLiteral());

    EXPECT_FALSE(error::hasError());

    DataTree::expectOne()
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
    misc::position pos_d(300);
    util::sptr<proto::Block> block(new proto::Block);
    flchk::GlobalFilter filter;
    filter.defFunc(pos_d, "fib", std::vector<std::string>(), util::mkptr(
                                                new flchk::FuncBodyFilter(filter.getSymbols())));

    std::vector<util::sptr<flchk::Expression const>> params;
    flchk::Call Call0(pos, "fib", std::move(params));

    params.push_back(util::mkptr(new flchk::BoolLiteral(pos, false)));
    params.push_back(util::mkptr(new flchk::PreUnaryOp(pos, "-", util::mkptr(
                                            new flchk::FloatLiteral(pos, "11.11")))));
    params.push_back(util::mkptr(new flchk::Negation(pos, util::mkptr(
                                            new flchk::IntLiteral(pos, "21")))));
    params.push_back(util::mkptr(new flchk::Reference(pos, "darekatasukete")));
    flchk::Call Call1(pos, "leap", std::move(params));

    Call0.compile(*block, filter.getSymbols())->inst(nul_st);
    EXPECT_FALSE(Call0.isLiteral());

    Call1.compile(*block, filter.getSymbols())->inst(nul_st);
    EXPECT_FALSE(Call1.isLiteral());

    EXPECT_FALSE(error::hasError());

    DataTree::expectOne()
        (pos_d, FUNC_DECL, "fib", 0, true)
        (pos, CALL, "fib", 0, false)
        (pos, FUNCTOR, "leap", 4, false)
            (pos, BOOLEAN, "false")
            (pos, PRE_UNARY_OP, "-")
                (pos, FLOATING, "11.11")
            (pos, PRE_UNARY_OP, "!")
                (pos, INTEGER, "21")
            (pos, REFERENCE, "darekatasukete")
    ;
}

TEST_F(ExprNodesTest, FuncReference)
{
    misc::position pos(4);
    util::sptr<proto::Block> block(new proto::Block);
    flchk::GlobalFilter filter;
    misc::position pos_d(400);
    filter.defFunc(pos_d, "fib", std::vector<std::string>(), util::mkptr(
                                                new flchk::FuncBodyFilter(filter.getSymbols())));
    filter.defFunc(pos_d, "fib", std::vector<std::string>({ "x" }), util::mkptr(
                                                new flchk::FuncBodyFilter(filter.getSymbols())));

    flchk::FuncReference func_ref0(pos, "fib", 0);
    flchk::FuncReference func_ref1(pos, "fib", 1);
    flchk::FuncReference func_ref2(pos, "fib", 0);

    func_ref0.compile(*block, filter.getSymbols())->inst(nul_st);
    EXPECT_FALSE(func_ref0.isLiteral());

    func_ref1.compile(*block, filter.getSymbols())->inst(nul_st);
    EXPECT_FALSE(func_ref0.isLiteral());

    func_ref2.compile(*block, filter.getSymbols())->inst(nul_st);
    EXPECT_FALSE(func_ref0.isLiteral());

    EXPECT_FALSE(error::hasError());

    DataTree::expectOne()
        (pos_d, FUNC_DECL, "fib", 0, true)
        (pos, FUNC_REFERENCE, "fib", 0, false)
        (pos_d, FUNC_DECL, "fib", 1, true)
            (pos_d, PARAMETER, "x")
        (pos, FUNC_REFERENCE, "fib", 1, false)
        (pos, FUNC_REFERENCE, "fib", 0, false)
    ;
}

TEST_F(ExprNodesTest, LiteralBoolValueError)
{
    misc::position pos(5);
    flchk::IntLiteral int0(pos, "20110409");
    int0.boolValue();

    flchk::FloatLiteral float0(pos, "10.58");
    float0.boolValue();

    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(2, getCondNotBools().size());
    EXPECT_EQ(pos, getCondNotBools()[0].pos);
    EXPECT_EQ("(int(20110409))", getCondNotBools()[0].type_name);
    EXPECT_EQ(pos, getCondNotBools()[1].pos);
    EXPECT_EQ("(float(10.58))", getCondNotBools()[1].type_name);
}

TEST_F(ExprNodesTest, OperationLiteralBoolValueError)
{
    misc::position pos(6);
    flchk::Conjunction conj(pos
                          , std::move(util::mkptr(new flchk::BoolLiteral(pos, true)))
                          , std::move(util::mkptr(new flchk::FloatLiteral(pos, "20110.4"))));
    conj.boolValue();
    flchk::Disjunction disj(pos
                          , std::move(util::mkptr(new flchk::BoolLiteral(pos, false)))
                          , std::move(util::mkptr(new flchk::IntLiteral(pos, "2"))));
    disj.boolValue();
    flchk::Negation nega(pos, std::move(util::mkptr(new flchk::FloatLiteral(pos, "1.12"))));
    nega.boolValue();

    flchk::BinaryOp binary(pos
                         , std::move(util::mkptr(new flchk::IntLiteral(pos, "1")))
                         , "*"
                         , std::move(util::mkptr(new flchk::FloatLiteral(pos, "11235.8"))));
    binary.boolValue();

    flchk::PreUnaryOp pre_unary(pos
                              , "+"
                              , std::move(util::mkptr(new flchk::FloatLiteral(pos, ".13"))));
    pre_unary.boolValue();

    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(5, getCondNotBools().size());
    EXPECT_EQ(pos, getCondNotBools()[0].pos);
    EXPECT_EQ("(float(20110.4))", getCondNotBools()[0].type_name);
    EXPECT_EQ(pos, getCondNotBools()[1].pos);
    EXPECT_EQ("(int(2))", getCondNotBools()[1].type_name);
    EXPECT_EQ(pos, getCondNotBools()[2].pos);
    EXPECT_EQ("(float(1.12))", getCondNotBools()[2].type_name);
    EXPECT_EQ(pos, getCondNotBools()[3].pos);
    EXPECT_EQ("((int(1))*(float(11235.8)))", getCondNotBools()[3].type_name);
    EXPECT_EQ(pos, getCondNotBools()[4].pos);
    EXPECT_EQ("(+(float(0.13)))", getCondNotBools()[4].type_name);
}
