#include <gtest/gtest.h>

#include "phony-err-report.h"
#include "../function.h"
#include "../../misc/pos-type.h"

using namespace test;

struct FunctionTest
    : public testing::Test
{
    void SetUp()
    {
        clear_err();
    }
};

inst::type const TEST_T("test_t", 1);

TEST_F(FunctionTest, Resolved)
{
    std::vector<ret_type_conflict_rec> ret_type_conflicts;
    std::vector<inst::type const*> arg_types;
    std::map<std::string, inst::variable const> extvars;
    util::sref<inst::function> func = inst::function::create_instance(arg_types, extvars, true);
    ASSERT_TRUE(bool(func));
    ASSERT_EQ(0, func->arg_types.size());
    ASSERT_EQ(0, func->external_vars.size());
    ASSERT_FALSE(inst::has_error());

    ASSERT_TRUE(func->is_return_type_resolved());
    ASSERT_EQ(inst::type::BIT_VOID, func->get_return_type());
    ASSERT_FALSE(inst::has_error());

    func->set_return_type(misc::pos_type(1), inst::type::BIT_VOID);
    ASSERT_TRUE(func->is_return_type_resolved());
    ASSERT_FALSE(inst::has_error());

    func->set_return_type(misc::pos_type(2), &TEST_T);
    ASSERT_TRUE(func->is_return_type_resolved());
    ASSERT_TRUE(inst::has_error());
    ret_type_conflicts = get_ret_type_conflicts();
    ASSERT_EQ(1, ret_type_conflicts.size());
    ASSERT_EQ(misc::pos_type(2), ret_type_conflicts[0].this_pos);
    ASSERT_EQ(inst::type::BIT_VOID->name, ret_type_conflicts[0].prev_type_name);
    ASSERT_EQ(TEST_T.name, ret_type_conflicts[0].this_type_name);
}

TEST_F(FunctionTest, Unresolved)
{
    std::vector<ret_type_conflict_rec> ret_type_conflicts;
    std::vector<inst::type const*> arg_types;
    std::map<std::string, inst::variable const> extvars;
    util::sref<inst::function> func = inst::function::create_instance(arg_types, extvars, false);
    ASSERT_TRUE(bool(func));
    ASSERT_EQ(0, func->arg_types.size());
    ASSERT_EQ(0, func->external_vars.size());
    ASSERT_FALSE(inst::has_error());
    ASSERT_FALSE(func->is_return_type_resolved());

    func->set_return_type(misc::pos_type(10), &TEST_T);
    ASSERT_TRUE(func->is_return_type_resolved());
    ASSERT_EQ(&TEST_T, func->get_return_type());
    ASSERT_FALSE(inst::has_error());

    func->set_return_type(misc::pos_type(20), inst::type::BIT_VOID);
    ASSERT_TRUE(func->is_return_type_resolved());
    ASSERT_TRUE(inst::has_error());
    ret_type_conflicts = get_ret_type_conflicts();
    ASSERT_EQ(1, ret_type_conflicts.size());
    ASSERT_EQ(misc::pos_type(20), ret_type_conflicts[0].this_pos);
    ASSERT_EQ(TEST_T.name, ret_type_conflicts[0].prev_type_name);
    ASSERT_EQ(inst::type::BIT_VOID->name, ret_type_conflicts[0].this_type_name);
}
