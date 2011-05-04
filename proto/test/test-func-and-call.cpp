#include <gtest/gtest.h>

#include "test-common.h"
#include "../scope.h"
#include "../function.h"
#include "../stmt-nodes.h"
#include "../expr-nodes.h"
#include "../../util/string.h"
#include "../../test/phony-errors.h"

using namespace test;

struct FuncNCallTest
    : public ProtoTest
{
    void SetUp()
    {
        ProtoTest::SetUp();
        resetFunc(true);
        inst_scope.reset(new PhonyFunc);
    }

    void resetFunc(bool hint_return_void)
    {
        misc::position pos(65535);
        func.reset(new proto::Function(pos, "f", std::vector<std::string>(), hint_return_void));
    }

    util::sptr<proto::Function> func;
    util::sptr<inst::Scope> inst_scope;
};

TEST_F(FuncNCallTest, NoBranchRecursionFunc)
{
    misc::position pos(1);
    func->inst(pos, *inst_scope, std::vector<util::sref<inst::Type const>>());
    ASSERT_FALSE(error::hasError());

    resetFunc(true);
    func->addStmt(util::mkptr(new proto::ReturnNothing(pos)));
    func->inst(pos, *inst_scope, std::vector<util::sref<inst::Type const>>());
    ASSERT_FALSE(error::hasError());

    resetFunc(false);
    func->addStmt(util::mkptr(new proto::Return(pos, util::mkptr(
                                                    new proto::IntLiteral(pos, 20110127)))));
    func->inst(pos, *inst_scope, std::vector<util::sref<inst::Type const>>());
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
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
    util::sptr<proto::Scope> sub_scope0(NULL);
    util::sptr<proto::Scope> sub_scope1(NULL);
    util::sref<proto::Function> test_func(NULL);

    test_func = func->declare(pos, "test_func", std::vector<std::string>(), false);
    sub_scope0 = util::mkptr(new proto::Scope);
    sub_scope1 = util::mkptr(new proto::Scope);

    util::sptr<proto::Expression const> recursive_call(util::mkptr(new proto::Call(
                                   pos
                                 , test_func
                                 , std::vector<util::sptr<proto::Expression const>>())));
    sub_scope0->addStmt(std::move(util::mkptr(new proto::Return(pos, std::move(recursive_call)))));
    ASSERT_FALSE(error::hasError());
    test_func->addStmt(util::mkptr(new proto::Branch(pos
                                                   , util::mkptr(new proto::IntLiteral(pos, 1))
                                                   , std::move(sub_scope0)
                                                   , std::move(sub_scope1))));
    test_func->addStmt(util::mkptr(new proto::Return(pos, util::mkptr(
                                                            new proto::IntLiteral(pos, 1)))));

    test_func->inst(pos, *inst_scope, std::vector<util::sref<inst::Type const>>());
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
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
    util::sptr<proto::Scope> sub_scope0(NULL);
    util::sptr<proto::Scope> sub_scope1(NULL);
    util::sref<proto::Function> test_func(NULL);

    test_func = func->declare(pos, "test_func", std::vector<std::string>(), false);
    sub_scope0 = util::mkptr(new proto::Scope);
    sub_scope1 = util::mkptr(new proto::Scope);

    test_func->addStmt(util::mkptr(new proto::Branch(pos
                                                   , util::mkptr(new proto::IntLiteral(pos, 1))
                                                   , std::move(sub_scope0)
                                                   , std::move(sub_scope1))));

    util::sptr<proto::Expression const> recursive_call(new proto::Call(
                                   pos
                                 , test_func
                                 , std::vector<util::sptr<proto::Expression const>>()));
    test_func->addStmt(std::move(util::mkptr(new proto::Return(pos, std::move(recursive_call)))));
    ASSERT_FALSE(error::hasError());

    test_func->inst(pos, *inst_scope, std::vector<util::sref<inst::Type const>>());
    ASSERT_TRUE(error::hasError());

    DataTree::expectOne()
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
