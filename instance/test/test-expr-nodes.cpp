#include <gtest/gtest.h>

#include <util/string.h>
#include <util/sn.h>
#include <misc/platform.h>

#include "test-common.h"
#include "../expr-nodes.h"

using namespace test;

typedef InstanceTest ExprNodesTest;

TEST_F(ExprNodesTest, SimpleLiterals)
{
    inst::IntLiteral i0(0);
    inst::IntLiteral i1(0x7fffffffffffffff);
    inst::FloatLiteral f0(3.14);
    inst::FloatLiteral f1(-2.72);
    inst::BoolLiteral b0(false);
    inst::BoolLiteral b1(true);
    inst::StringLiteral str("nostalgia");

    i0.write();
    i1.write();
    f0.write();
    f1.write();
    b0.write();
    b1.write();
    str.write();

    DataTree::expectOne()
        (INTEGER, "0")
        (INTEGER, util::str(platform::int_type(0x7fffffffffffffff)))
        (FLOAT, "3.14")
        (FLOAT, "-2.72")
        (BOOLEAN, "false")
        (BOOLEAN, "true")
        (STRING, "nostalgia", 9)
    ;
}

TEST_F(ExprNodesTest, ListLiterals)
{
    inst::ListLiteral ls0(util::mkptr(new inst::IntPrimitive)
                        , std::vector<util::sptr<inst::Expression const>>());
    ls0.write();

    std::vector<util::sptr<inst::Expression const>> members;
    members.push_back(util::mkptr(new inst::BoolLiteral(false)));
    members.push_back(util::mkptr(new inst::BoolLiteral(true)));
    inst::ListLiteral ls1(util::mkptr(new inst::BoolPrimitive), std::move(members));
    ls1.write();

    inst::EmptyListLiteral ls2;
    ls2.write();

    DataTree::expectOne()
        (LIST_BEGIN, "int", 0)
        (LIST_END)
        (LIST_BEGIN, "bool", 2)
            (LIST_NEXT_MEMBER)
            (BOOLEAN, "false")
            (LIST_NEXT_MEMBER)
            (BOOLEAN, "true")
        (LIST_END)
        (EMPTY_LIST)
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

TEST_F(ExprNodesTest, Call)
{
    std::vector<util::sptr<inst::Expression const>> args;
    inst::Call c0(util::serial_num::next(), std::move(args));

    args.push_back(util::mkptr(new inst::IntLiteral(20110724)));
    args.push_back(util::mkptr(new inst::Reference(util::mkptr(new inst::VoidPrimitive)
                                                 , inst::Address(0, 8))));
    inst::Call c1(util::serial_num::next(), std::move(args));

    c0.write();
    c1.write();

    DataTree::expectOne()
        (CALL_BEGIN)
        (CALL_END)
        (CALL_BEGIN)
            (ARG_SEPARATOR)
            (INTEGER, "20110724")
            (ARG_SEPARATOR)
            (REFERENCE, "void", 0, 8)
        (CALL_END)
    ;
}

TEST_F(ExprNodesTest, FuncReference)
{
    std::list<inst::FuncReference::ArgInfo> args;
    inst::FuncReference r0(0, std::move(args));

    args.push_back(inst::FuncReference::ArgInfo(inst::Address(1, 0)
                                              , util::mkptr(new inst::IntPrimitive)
                                              , 0));
    args.push_back(inst::FuncReference::ArgInfo(inst::Address(1, 8)
                                              , util::mkptr(new inst::FloatPrimitive)
                                              , 8));
    inst::FuncReference r1(16, std::move(args));

    r0.write();
    r1.write();

    DataTree::expectOne()
        (FUNC_REFERENCE, "0")
        (FUNC_REFERENCE, "16")
            (FUNC_REF_NEXT_VAR, "int", 1, 0, 0)
            (FUNC_REF_NEXT_VAR, "float", 1, 8, 8)
    ;
}

TEST_F(ExprNodesTest, Arithmetics)
{
    inst::BinaryOp op0(util::mkptr(new inst::IntLiteral(20110728))
                     , "%"
                     , util::mkptr(new inst::IntLiteral(2306)));
    inst::BinaryOp op1(util::mkptr(new inst::BoolLiteral(true))
                     , "=="
                     , util::mkptr(new inst::BoolLiteral(false)));
    inst::PreUnaryOp op2("+", util::mkptr(new inst::IntLiteral(19880317)));
    inst::PreUnaryOp op3("-", util::mkptr(new inst::FloatLiteral(23.08)));

    op0.write();
    op1.write();
    op2.write();
    op3.write();

    DataTree::expectOne()
        (EXPRESSION_BEGIN)
            (INTEGER, "20110728")
            (OPERATOR, "%")
            (INTEGER, "2306")
        (EXPRESSION_END)

        (EXPRESSION_BEGIN)
            (BOOLEAN, "true")
            (OPERATOR, "==")
            (BOOLEAN, "false")
        (EXPRESSION_END)

        (EXPRESSION_BEGIN)
            (OPERATOR, "+")
            (INTEGER, "19880317")
        (EXPRESSION_END)

        (EXPRESSION_BEGIN)
            (OPERATOR, "-")
            (FLOAT, "23.08")
        (EXPRESSION_END)
    ;
}

TEST_F(ExprNodesTest, Logics)
{
    inst::Negation nega(util::mkptr(new inst::BoolLiteral(true)));
    inst::Conjunction conj(util::mkptr(new inst::BoolLiteral(true))
                         , util::mkptr(new inst::BoolLiteral(false)));
    inst::Disjunction disj(util::mkptr(new inst::BoolLiteral(false))
                         , util::mkptr(new inst::BoolLiteral(false)));

    nega.write();
    conj.write();
    disj.write();

    DataTree::expectOne()
        (EXPRESSION_BEGIN)
            (OPERATOR, "!")
            (BOOLEAN, "true")
        (EXPRESSION_END)

        (EXPRESSION_BEGIN)
            (BOOLEAN, "true")
            (OPERATOR, "&&")
            (BOOLEAN, "false")
        (EXPRESSION_END)

        (EXPRESSION_BEGIN)
            (BOOLEAN, "false")
            (OPERATOR, "||")
            (BOOLEAN, "false")
        (EXPRESSION_END)
    ;
}
