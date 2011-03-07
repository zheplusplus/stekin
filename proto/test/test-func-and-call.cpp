#include <gtest/gtest.h>

#include "test-common.h"
#include "../scope.h"
#include "../symbol-table.h"
#include "../func-templ.h"
#include "../../util/string.h"
#include "../../test/phony-errors.h"

using namespace test;

struct FuncNCallTest
    : public proto_test
{
    void SetUp()
    {
        proto_test::SetUp();
        reset_func();
        inst_scope.reset(new phony_func);
    }

    void reset_func()
    {
        misc::pos_type pos(65535);
        func.reset(new proto::func_templ(pos, "f", std::vector<std::string>()));
    }

    util::sptr<proto::func_templ> func;
    util::sptr<inst::scope> inst_scope;
};

TEST_F(FuncNCallTest, NoBranchRecursionFunc)
{
    misc::pos_type pos(1);
    func->inst(pos, *inst_scope, std::vector<inst::type const*>());
    ASSERT_FALSE(error::has_error());

    reset_func();
    func->add_func_ret_nothing(pos);
    func->inst(pos, *inst_scope, std::vector<inst::type const*>());
    ASSERT_FALSE(error::has_error());

    reset_func();
    func->add_func_ret(pos, std::move(func->make_int(pos, "20110127")));
    func->inst(pos, *inst_scope, std::vector<inst::type const*>());
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
    misc::pos_type pos(2);
    util::sptr<proto::scope> sub_scope0(NULL);
    util::sptr<proto::scope> sub_scope1(NULL);
    util::sref<proto::func_templ> test_func(NULL);

    test_func = func->decl_func(pos, "test_func", std::vector<std::string>());
    sub_scope0 = std::move(test_func->create_branch_scope());
    sub_scope1 = std::move(test_func->create_branch_scope());

    sub_scope0->add_func_ret(pos, std::move(
                        sub_scope0->make_call(pos
                                            , "test_func"
                                            , std::move(std::vector<util::sptr<proto::expr_base const>>()))));
    ASSERT_FALSE(error::has_error());
    test_func->add_branch(pos
                        , std::move(test_func->make_int(pos, "1"))
                        , std::move(sub_scope0)
                        , std::move(sub_scope1));
    test_func->add_func_ret(pos, std::move(test_func->make_int(pos, "1")));

    test_func->inst(pos, *inst_scope, std::vector<inst::type const*>());
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
    misc::pos_type pos(3);
    util::sptr<proto::scope> sub_scope0(NULL);
    util::sptr<proto::scope> sub_scope1(NULL);
    util::sref<proto::func_templ> test_func(NULL);

    test_func = func->decl_func(pos, "test_func", std::vector<std::string>());
    sub_scope0 = std::move(test_func->create_branch_scope());
    sub_scope1 = std::move(test_func->create_branch_scope());

    test_func->add_branch(pos
                        , std::move(test_func->make_int(pos, "1"))
                        , std::move(sub_scope0)
                        , std::move(sub_scope1));
    test_func->add_func_ret(pos, std::move(
                         test_func->make_call(pos
                                            , "test_func"
                                            , std::move(std::vector<util::sptr<proto::expr_base const>>()))));
    ASSERT_FALSE(error::has_error());

    test_func->inst(pos, *inst_scope, std::vector<inst::type const*>());
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
