#include <gtest/gtest.h>

#include "test-common.h"
#include "../node-base.h"
#include "../function.h"
#include "../../misc/pos-type.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"

using namespace test;

struct FunctionTest
    : public testing::Test
{
    void SetUp()
    {
        clearErr();
    }
};

inst::BuiltInPrimitive const TEST_T("test_t", 1);

TEST_F(FunctionTest, Resolved)
{
    std::vector<RetTypeConflictRec> ret_type_conflicts;
    std::list<inst::ArgNameTypeRec> args;
    std::map<std::string, inst::Variable const> extvars;
    util::sref<inst::Function> func = inst::Function::createInstance(0, args, extvars, true);
    ASSERT_TRUE(func.not_nul());
    ASSERT_FALSE(error::hasError());

    ASSERT_TRUE(func->isReturnTypeResolved());
    ASSERT_EQ(inst::Type::BIT_VOID, func->getReturnType());
    ASSERT_FALSE(error::hasError());

    func->setReturnType(misc::position(1), inst::Type::BIT_VOID);
    ASSERT_TRUE(func->isReturnTypeResolved());
    ASSERT_FALSE(error::hasError());

    func->setReturnType(misc::position(2), util::mkref(TEST_T));
    ASSERT_TRUE(func->isReturnTypeResolved());
    ASSERT_TRUE(error::hasError());
    ret_type_conflicts = getRetTypeConflicts();
    ASSERT_EQ(1, ret_type_conflicts.size());
    ASSERT_EQ(misc::position(2), ret_type_conflicts[0].this_pos);
    ASSERT_EQ(inst::Type::BIT_VOID->name(), ret_type_conflicts[0].prev_type_name);
    ASSERT_EQ(TEST_T.name(), ret_type_conflicts[0].this_type_name);
}

TEST_F(FunctionTest, Unresolved)
{
    std::vector<RetTypeConflictRec> ret_type_conflicts;
    std::list<inst::ArgNameTypeRec> args;
    std::map<std::string, inst::Variable const> extvars;
    util::sref<inst::Function> func = inst::Function::createInstance(0, args, extvars, false);
    ASSERT_TRUE(func.not_nul());
    ASSERT_FALSE(error::hasError());
    ASSERT_FALSE(func->isReturnTypeResolved());

    func->setReturnType(misc::position(10), util::mkref(TEST_T));
    ASSERT_TRUE(func->isReturnTypeResolved());
    ASSERT_EQ(util::sref<inst::Type const>(&TEST_T), func->getReturnType());
    ASSERT_FALSE(error::hasError());

    func->setReturnType(misc::position(20), inst::Type::BIT_VOID);
    ASSERT_TRUE(func->isReturnTypeResolved());
    ASSERT_TRUE(error::hasError());
    ret_type_conflicts = getRetTypeConflicts();
    ASSERT_EQ(1, ret_type_conflicts.size());
    ASSERT_EQ(misc::position(20), ret_type_conflicts[0].this_pos);
    ASSERT_EQ(TEST_T.name(), ret_type_conflicts[0].prev_type_name);
    ASSERT_EQ(inst::Type::BIT_VOID->name(), ret_type_conflicts[0].this_type_name);
}
