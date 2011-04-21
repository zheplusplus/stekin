#include <gtest/gtest.h>

#include "test-common.h"
#include "../scope.h"
#include "../symbol-table.h"
#include "../function.h"
#include "../stmt-nodes.h"
#include "../../util/string.h"
#include "../../test/phony-errors.h"

using namespace test;

struct FuncNCallTest
    : public proto_test
{
    void SetUp()
    {
        proto_test::SetUp();
        reset_func(true);
        inst_scope.reset(new phony_func);
    }

    void reset_func(bool hint_func_return_void)
    {
        ext_symbols.reset(new proto::symbol_table);
        misc::position pos(65535);
        func.reset(new proto::Function(pos
                                     , "f"
                                     , std::vector<std::string>()
                                     , *ext_symbols
                                     , hint_func_return_void));
    }

    util::sptr<proto::symbol_table> ext_symbols;
    util::sptr<proto::Function> func;
    util::sptr<inst::scope> inst_scope;
};

TEST_F(FuncNCallTest, NoBranchRecursionFunc)
{
    misc::position pos(1);
    func->inst(pos, *inst_scope, std::vector<util::sref<inst::type const>>());
    ASSERT_FALSE(error::has_error());

    reset_func(true);
    func->add_stmt(std::move(util::mkptr(new proto::func_ret_nothing(pos))));
    func->inst(pos, *inst_scope, std::vector<util::sref<inst::type const>>());
    ASSERT_FALSE(error::has_error());

    reset_func(false);
    func->add_stmt(std::move(
                util::mkptr(new proto::func_ret(pos, std::move(func->make_int(pos, 20110127))))));
    func->inst(pos, *inst_scope, std::vector<util::sref<inst::type const>>());
    ASSERT_FALSE(error::has_error());

    data_tree::expect_one()
        (INIT_AS_VOID_RET)
        (ADD_PATH)
        (NEXT_PATH)
        (ADD_STMT_TO_SCOPE)

        (INIT_AS_VOID_RET)
        (ADD_PATH)
        (NEXT_PATH)
            (pos, SET_RETURN_TYPE_VOID)
        (ADD_STMT_TO_BLOCK)
        (ADD_STMT_TO_SCOPE)

        (ADD_PATH)
        (NEXT_PATH)
                (INTEGER, "20110127")
            (pos, SET_RETURN_TYPE)
        (ADD_STMT_TO_BLOCK)
        (ADD_STMT_TO_SCOPE)
    ;
}

TEST_F(FuncNCallTest, FuncWithBranchRecursion)
{
    misc::position pos(2);
    util::sptr<proto::scope> sub_scope0(NULL);
    util::sptr<proto::scope> sub_scope1(NULL);
    util::sref<proto::Function> test_func(NULL);

    test_func = func->declare(pos, "test_func", std::vector<std::string>(), false);
    sub_scope0 = std::move(test_func->create_branch_scope());
    sub_scope1 = std::move(test_func->create_branch_scope());

    util::sptr<proto::Expression const> recursive_call(
            std::move(sub_scope0->make_call(pos
                                          , "test_func"
                                          , std::move(std::vector<util::sptr<proto::Expression const>>()))));
    sub_scope0->add_stmt(std::move(util::mkptr(new proto::func_ret(pos, std::move(recursive_call)))));
    ASSERT_FALSE(error::has_error());
    test_func->add_stmt(std::move(
                util::mkptr(new proto::branch(pos
                                            , std::move(test_func->make_int(pos, 1))
                                            , std::move(sub_scope0->deliver())
                                            , std::move(sub_scope1->deliver())))));
    test_func->add_stmt(std::move(
                util::mkptr(new proto::func_ret(pos, std::move(test_func->make_int(pos, 1))))));

    test_func->inst(pos, *inst_scope, std::vector<util::sref<inst::type const>>());
    ASSERT_FALSE(error::has_error());

    data_tree::expect_one()
        (ADD_PATH)
        (NEXT_PATH)
        (ADD_PATH)
        (ADD_PATH)
            (INTEGER, "1")
        (pos, SET_RETURN_TYPE)

                (QUERY_RETURN_TYPE_RESOLVE_STATUS)
                (QUERY_RETURN_TYPE_RESOLVE_STATUS)
                    (CALL, util::str(0))
                (pos, SET_RETURN_TYPE)
            (ADD_STMT_TO_BLOCK)
        (ADD_STMT_TO_BLOCK)
        (ADD_STMT_TO_BLOCK)
        (ADD_STMT_TO_SCOPE)
    ;
}

TEST_F(FuncNCallTest, CouldNotResolve)
{
    misc::position pos(3);
    util::sptr<proto::scope> sub_scope0(NULL);
    util::sptr<proto::scope> sub_scope1(NULL);
    util::sref<proto::Function> test_func(NULL);

    test_func = func->declare(pos, "test_func", std::vector<std::string>(), false);
    sub_scope0 = std::move(test_func->create_branch_scope());
    sub_scope1 = std::move(test_func->create_branch_scope());

    test_func->add_stmt(std::move(
                util::mkptr(new proto::branch(pos
                                            , std::move(test_func->make_int(pos, 1))
                                            , std::move(sub_scope0->deliver())
                                            , std::move(sub_scope1->deliver())))));

    util::sptr<proto::Expression const> recursive_call(
            std::move(test_func->make_call(pos
                                         , "test_func"
                                         , std::move(std::vector<util::sptr<proto::Expression const>>()))));
    test_func->add_stmt(std::move(util::mkptr(new proto::func_ret(pos, std::move(recursive_call)))));
    ASSERT_FALSE(error::has_error());

    test_func->inst(pos, *inst_scope, std::vector<util::sref<inst::type const>>());
    ASSERT_TRUE(error::has_error());

    data_tree::expect_one()
        (ADD_PATH)
        (NEXT_PATH)
        (ADD_PATH)
        (ADD_PATH)

        (QUERY_RETURN_TYPE_RESOLVE_STATUS)
        (NEXT_PATH)
        (QUERY_RETURN_TYPE_RESOLVE_STATUS)
        (NEXT_PATH)

        (QUERY_RETURN_TYPE_RESOLVE_STATUS)
        (QUERY_RETURN_TYPE_RESOLVE_STATUS)

                (CALL, util::str(0))
            (pos, SET_RETURN_TYPE)
            (ADD_STMT_TO_BLOCK)
        (ADD_STMT_TO_BLOCK)
        (ADD_STMT_TO_SCOPE)
    ;
}
