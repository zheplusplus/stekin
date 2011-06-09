#include <gtest/gtest.h>

#include "test-common.h"
#include "../function.h"
#include "../symbol-table.h"
#include "../expr-nodes.h"
#include "../../util/string.h"
#include "../../test/phony-errors.h"

using namespace test;

struct FuncNCallTest
    : public ProtoTest
{
    FuncNCallTest()
        : global_st(NULL)
        , block(NULL)
    {}

    void SetUp()
    {
        ProtoTest::SetUp();
        global_st.reset(new proto::SymbolTable);
        block.reset(new proto::Block);
    }

    util::sptr<proto::SymbolTable> global_st;
    util::sptr<proto::Block> block;
};

TEST_F(FuncNCallTest, EmptyBodyFunc)
{
    misc::position pos(1);
    util::sref<proto::Function> func(
            block->declare(pos, "empty_body", std::vector<std::string>(), true));
    ASSERT_FALSE(error::hasError());

    proto::Call call(pos, func, std::vector<util::sptr<proto::Expression const>>());
    call.inst(*global_st)->write();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (CALL, "0")
    ;
}

TEST_F(FuncNCallTest, NoBranchRecursionFunc)
{
    misc::position pos(2);
    util::sref<proto::Function> func(
            block->declare(pos, "first", std::vector<std::string>(), true));
    ASSERT_FALSE(error::hasError());

    BlockFiller(func->block())
        .ret(pos)
    ;
    proto::Call call_first(pos, func, std::vector<util::sptr<proto::Expression const>>());
    call_first.inst(*global_st)->write();
    ASSERT_FALSE(error::hasError());

    func = block->declare(pos, "second", std::vector<std::string>(), false);
    BlockFiller(func->block())
        .ret(pos, util::mkptr(new proto::IntLiteral(pos, mpz_class(20110127))))
    ;
    proto::Call call_second(pos, func, std::vector<util::sptr<proto::Expression const>>());
    call_second.inst(*global_st)->write();
    ASSERT_FALSE(error::hasError());

    func = block->declare(pos, "second", std::vector<std::string>({ "x" }), false);
    BlockFiller(func->block())
        .ret(pos, util::mkptr(new proto::Reference(pos, "x")))
    ;
    std::vector<util::sptr<proto::Expression const>> args;
    args.push_back(util::mkptr(new proto::BoolLiteral(pos, false)));
    proto::Call call_second_x(pos, func, std::move(args));
    call_second_x.inst(*global_st)->write();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (CALL, "0")
        (CALL, "0")
        (CALL, "1")
    ;
}

TEST_F(FuncNCallTest, FuncWithBranchRecursion)
{
    misc::position pos(3);
    util::sptr<proto::Block> sub0(new proto::Block);
    util::sptr<proto::Block> sub1(new proto::Block);
    util::sref<proto::Function> test_func(
            block->declare(pos, "test_func", std::vector<std::string>({ "x" }), false));

    std::vector<util::sptr<proto::Expression const>> args;
    args.push_back(util::mkptr(new proto::BoolLiteral(pos, true)));
    BlockFiller(*sub0)
        .ret(pos, util::mkptr(new proto::Call(pos, test_func, std::move(args))))
    ;
    ASSERT_FALSE(error::hasError());
    BlockFiller(test_func->block())
        .branch(pos, util::mkptr(new proto::Reference(pos, "x")), std::move(sub0), std::move(sub1))
        .ret(pos, util::mkptr(new proto::BoolLiteral(pos, false)))
    ;

    args.push_back(util::mkptr(new proto::BoolLiteral(pos, false)));
    proto::Call call(pos, test_func, std::move(args));
    call.inst(*global_st)->write();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (CALL, "1")
    ;
}

TEST_F(FuncNCallTest, CouldNotResolve)
{
    misc::position pos(4);
    util::sptr<proto::Block> sub0(new proto::Block);
    util::sptr<proto::Block> sub1(new proto::Block);
    util::sref<proto::Function> test_func(
            block->declare(pos, "test_func", std::vector<std::string>(), false));

    BlockFiller(test_func->block())
        .ret(pos, util::mkptr(new proto::Call(pos
                                            , test_func
                                            , std::vector<util::sptr<proto::Expression const>>())))
    ;
    ASSERT_FALSE(error::hasError());

    proto::Call call(pos, test_func, std::vector<util::sptr<proto::Expression const>>());
    call.inst(*global_st);
    EXPECT_TRUE(error::hasError());
    ASSERT_EQ(1, getRetTypeUnresolvables().size());
    ASSERT_EQ("test_func", getRetTypeUnresolvables()[0].name);
    ASSERT_EQ(0, getRetTypeUnresolvables()[0].arg_count);
}

TEST_F(FuncNCallTest, WriteEmptyFunc)
{
    misc::position pos(5);
    util::sref<proto::Function> func(
            block->declare(pos, "empty_body", std::vector<std::string>(), true));
    ASSERT_FALSE(error::hasError());

    proto::Call call(pos, func, std::vector<util::sptr<proto::Expression const>>());
    call.inst(*global_st);

    std::vector<util::sptr<inst::Function const>> funcs(block->deliverFuncs());
    ASSERT_FALSE(error::hasError());

    ASSERT_EQ(1, funcs.size());
    funcs[0]->writeDecl();
    funcs[0]->writeImpl();

    DataTree::expectOne()
        (FUNC_DECL, "1", 0)
        (FUNC_IMPL, "1", 0)
            (BLOCK_BEGIN)
            (BLOCK_END)
    ;
}

TEST_F(FuncNCallTest, WriteNestedCallInBranches)
{
    misc::position pos(6);
    util::sptr<proto::Block> sub0(new proto::Block);
    util::sptr<proto::Block> sub1(new proto::Block);

    util::sref<proto::Function> func(
            block->declare(pos, "f0", std::vector<std::string>({ "x" }), false));
    util::sref<proto::Function> nested(
            func->block()->declare(pos, "n0", std::vector<std::string>(), false));

    std::vector<util::sptr<proto::Expression const>> call_args;
    call_args.push_back(util::mkptr(new proto::BoolLiteral(pos, true)));
    BlockFiller(nested->block())
        .ret(pos, util::mkptr(new proto::Call(pos, func, std::move(call_args))))
    ;

    BlockFiller(*sub0)
        .ret(pos, util::mkptr(new proto::Call(pos
                                            , nested
                                            , std::vector<util::sptr<proto::Expression const>>())))
    ;
    BlockFiller(func->block())
        .branch(pos, util::mkptr(new proto::Reference(pos, "x")), std::move(sub0), std::move(sub1))
        .ret(pos, util::mkptr(new proto::IntLiteral(pos, mpz_class(0))))
    ;
    ASSERT_FALSE(error::hasError());

    std::vector<util::sptr<proto::Expression const>> args;
    args.push_back(util::mkptr(new proto::BoolLiteral(pos, false)));
    proto::Call call(pos, func, std::move(args));
    call.inst(*global_st);

    std::vector<util::sptr<inst::Function const>> funcs(block->deliverFuncs());
    ASSERT_FALSE(error::hasError());

    ASSERT_EQ(2, funcs.size());
    funcs[0]->writeDecl();
    funcs[1]->writeDecl();
    funcs[0]->writeImpl();
    funcs[1]->writeImpl();

    DataTree::expectOne()
        (FUNC_DECL, "2", 0)
        (FUNC_DECL, "1", 1)
        (FUNC_IMPL, "2", 0)
            (BLOCK_BEGIN)
            (RETURN)
                (CALL, "1")
            (BLOCK_END)
        (FUNC_IMPL, "1", 1)
            (BLOCK_BEGIN)
            (BRANCH)
                (REFERENCE)
                (BLOCK_BEGIN)
                (RETURN)
                    (CALL, "0")
                (BLOCK_END)
                (BLOCK_BEGIN)
                (BLOCK_END)
            (RETURN)
                (INTEGER, "0")
            (BLOCK_END)
    ;
}

TEST_F(FuncNCallTest, WriteNestedCallOutsideBranches)
{
    misc::position pos(7);
    util::sptr<proto::Block> sub0(new proto::Block);
    util::sptr<proto::Block> sub1(new proto::Block);

    util::sref<proto::Function> func(
            block->declare(pos, "f0", std::vector<std::string>({ "x" }), false));
    util::sref<proto::Function> nested(
            func->block()->declare(pos, "n0", std::vector<std::string>(), false));

    std::vector<util::sptr<proto::Expression const>> call_args;
    call_args.push_back(util::mkptr(new proto::BoolLiteral(pos, true)));
    BlockFiller(nested->block())
        .ret(pos, util::mkptr(new proto::Call(pos, func, std::move(call_args))))
    ;

    BlockFiller(*sub0)
        .ret(pos, util::mkptr(new proto::IntLiteral(pos, mpz_class(0))))
    ;
    BlockFiller(func->block())
        .branch(pos, util::mkptr(new proto::Reference(pos, "x")), std::move(sub0), std::move(sub1))
        .ret(pos, util::mkptr(new proto::Call(pos
                                            , nested
                                            , std::vector<util::sptr<proto::Expression const>>())))
    ;
    ASSERT_FALSE(error::hasError());

    std::vector<util::sptr<proto::Expression const>> args;
    args.push_back(util::mkptr(new proto::BoolLiteral(pos, false)));
    proto::Call call(pos, func, std::move(args));
    call.inst(*global_st);

    std::vector<util::sptr<inst::Function const>> funcs(block->deliverFuncs());
    ASSERT_FALSE(error::hasError());

    ASSERT_EQ(2, funcs.size());
    funcs[0]->writeDecl();
    funcs[1]->writeDecl();
    funcs[0]->writeImpl();
    funcs[1]->writeImpl();

    DataTree::expectOne()
        (FUNC_DECL, "2", 0)
        (FUNC_DECL, "1", 1)
        (FUNC_IMPL, "2", 0)
            (BLOCK_BEGIN)
            (RETURN)
                (CALL, "1")
            (BLOCK_END)
        (FUNC_IMPL, "1", 1)
            (BLOCK_BEGIN)
            (BRANCH)
                (REFERENCE)
                (BLOCK_BEGIN)
                (RETURN)
                    (INTEGER, "0")
                (BLOCK_END)
                (BLOCK_BEGIN)
                (BLOCK_END)
            (RETURN)
                (CALL, "0")
            (BLOCK_END)
    ;
}
