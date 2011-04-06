#include <vector>
#include <set>
#include <algorithm>
#include <gtest/gtest.h>

#include "test-common.h"
#include "../operation.h"
#include "../type.h"
#include "../../misc/pos-type.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"

using namespace test;

struct QueryOperationTest
    : public testing::Test
{
    void SetUp()
    {
        clear_err();
    }
};

std::vector<std::string> const ARITH_BIN_OP_LIST({ "+", "-", "*", "/", "%" });
std::vector<std::string> const COMP_OP_LIST({ "=", "<=", "<", ">=", ">", "!=" });
std::vector<std::string> const ARITH_PRE_UNARY_OP_LIST({ "+", "-" });

TEST_F(QueryOperationTest, NoError)
{
    int lineno = 1;
    std::set<inst::operation const*> op_records;
    std::for_each(ARITH_BIN_OP_LIST.begin()
                , ARITH_BIN_OP_LIST.end()
                , [&](std::string const& op)
                  {
                      inst::operation const* result = inst::operation::query_binary(
                              misc::pos_type(lineno), op, inst::type::BIT_INT, inst::type::BIT_INT);
                      ASSERT_FALSE(error::has_error()) << "operation img=" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img=" << op << " duplicate operation.";
                      ASSERT_EQ(inst::type::BIT_INT, result->ret_type);
                      ++lineno;
                  });

    std::for_each(ARITH_BIN_OP_LIST.begin()
                , ARITH_BIN_OP_LIST.end() - 1
                , [&](std::string const& op)
                  {
                      inst::operation const* result = inst::operation::query_binary(
                              misc::pos_type(lineno), op, inst::type::BIT_FLOAT, inst::type::BIT_FLOAT);
                      ASSERT_FALSE(error::has_error()) << "operation img=" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img=" << op << " duplicate operation.";
                      ASSERT_EQ(inst::type::BIT_FLOAT, result->ret_type);
                      ++lineno;
                  });

    std::for_each(ARITH_PRE_UNARY_OP_LIST.begin()
                , ARITH_PRE_UNARY_OP_LIST.end()
                , [&](std::string const& op)
                  {
                      inst::operation const* result;

                      result = inst::operation::query_pre_unary(
                              misc::pos_type(lineno), op, inst::type::BIT_INT);
                      ASSERT_FALSE(error::has_error()) << "operation img=" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img=" << op << " duplicate operation.";
                      ASSERT_EQ(inst::type::BIT_INT, result->ret_type);
                      ++lineno;

                      result = inst::operation::query_pre_unary(
                              misc::pos_type(lineno), op, inst::type::BIT_FLOAT);
                      ASSERT_FALSE(error::has_error()) << "operation img=" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img=" << op << " duplicate operation.";
                      ASSERT_EQ(inst::type::BIT_FLOAT, result->ret_type);
                      ++lineno;
                  });

    std::for_each(COMP_OP_LIST.begin()
                , COMP_OP_LIST.end()
                , [&](std::string const& op)
                  {
                      inst::operation const* result;

                      result = inst::operation::query_binary(
                              misc::pos_type(lineno), op, inst::type::BIT_INT, inst::type::BIT_INT);
                      ASSERT_FALSE(error::has_error()) << "operation img=" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img=" << op << " duplicate operation.";
                      ASSERT_EQ(inst::type::BIT_BOOL, result->ret_type);
                      ++lineno;

                      result = inst::operation::query_binary(
                              misc::pos_type(lineno), op, inst::type::BIT_FLOAT, inst::type::BIT_FLOAT);
                      ASSERT_FALSE(error::has_error()) << "operation img=" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img=" << op << " duplicate operation.";
                      ASSERT_EQ(inst::type::BIT_BOOL, result->ret_type);
                      ++lineno;
                  });
    ASSERT_FALSE(error::has_error());
}

TEST_F(QueryOperationTest, BadInputType)
{
    int lineno = 1;
    std::vector<std::string> all_bin_op = ARITH_BIN_OP_LIST;
    all_bin_op.insert(all_bin_op.end(), COMP_OP_LIST.begin(), COMP_OP_LIST.end());
    std::for_each(all_bin_op.begin()
                , all_bin_op.end()
                , [&](std::string const& op)
                  {
                      inst::operation const* result;

                      result = inst::operation::query_binary(
                              misc::pos_type(lineno), op, inst::type::BAD_TYPE, inst::type::BAD_TYPE);
                      ASSERT_FALSE(error::has_error()) << "operation img=" << op << " query failed.";
                      ASSERT_EQ(inst::type::BAD_TYPE, result->ret_type);
                      ++lineno;

                      result = inst::operation::query_binary(
                              misc::pos_type(lineno), op, inst::type::BAD_TYPE, inst::type::BIT_INT);
                      ASSERT_FALSE(error::has_error()) << "operation img=" << op << " query failed.";
                      ASSERT_EQ(inst::type::BAD_TYPE, result->ret_type);
                      ++lineno;

                      result = inst::operation::query_binary(
                              misc::pos_type(lineno), op, inst::type::BIT_FLOAT, inst::type::BAD_TYPE);
                      ASSERT_FALSE(error::has_error()) << "operation img=" << op << " query failed.";
                      ASSERT_EQ(inst::type::BAD_TYPE, result->ret_type);
                      ++lineno;

                      result = inst::operation::query_binary(
                              misc::pos_type(lineno), op, inst::type::BAD_TYPE, inst::type::BIT_BOOL);
                      ASSERT_FALSE(error::has_error()) << "operation img=" << op << " query failed.";
                      ASSERT_EQ(inst::type::BAD_TYPE, result->ret_type);
                      ++lineno;

                      result = inst::operation::query_binary(
                              misc::pos_type(lineno), op, inst::type::BIT_VOID, inst::type::BAD_TYPE);
                      ASSERT_FALSE(error::has_error()) << "operation img=" << op << " query failed.";
                      ASSERT_EQ(inst::type::BAD_TYPE, result->ret_type);
                      ++lineno;
                  });

    std::for_each(ARITH_PRE_UNARY_OP_LIST.begin()
                , ARITH_PRE_UNARY_OP_LIST.end()
                , [&](std::string const& op)
                  {
                      inst::operation const* result = inst::operation::query_pre_unary(
                              misc::pos_type(lineno), op, inst::type::BAD_TYPE);
                      ASSERT_FALSE(error::has_error()) << "operation img=" << op << " query failed.";
                      ASSERT_EQ(inst::type::BAD_TYPE, result->ret_type);
                      ++lineno;
                  });
    ASSERT_FALSE(error::has_error());
}

TEST_F(QueryOperationTest, BadOperation)
{
    inst::operation const* result;

    result = inst::operation::query_binary(
            misc::pos_type(10), "%", inst::type::BIT_FLOAT, inst::type::BIT_FLOAT);
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_na_binary_ops().size());
    ASSERT_EQ(misc::pos_type(10), get_na_binary_ops()[0].pos);
    ASSERT_EQ("%", get_na_binary_ops()[0].op_img);
    ASSERT_EQ(inst::type::BIT_FLOAT->name(), get_na_binary_ops()[0].rhst_name);
    clear_err();

    result = inst::operation::query_binary(misc::pos_type(11), "<", inst::type::BIT_INT, inst::type::BIT_FLOAT);
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_na_binary_ops().size());
    ASSERT_EQ(misc::pos_type(11), get_na_binary_ops()[0].pos);
    ASSERT_EQ("<", get_na_binary_ops()[0].op_img);
    ASSERT_EQ(inst::type::BIT_INT->name(), get_na_binary_ops()[0].lhst_name);
    ASSERT_EQ(inst::type::BIT_FLOAT->name(), get_na_binary_ops()[0].rhst_name);
    clear_err();

    result = inst::operation::query_binary(misc::pos_type(12), "+", inst::type::BIT_INT, inst::type::BIT_BOOL);
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_na_binary_ops().size());
    ASSERT_EQ(misc::pos_type(12), get_na_binary_ops()[0].pos);
    ASSERT_EQ("+", get_na_binary_ops()[0].op_img);
    ASSERT_EQ(inst::type::BIT_INT->name(), get_na_binary_ops()[0].lhst_name);
    ASSERT_EQ(inst::type::BIT_BOOL->name(), get_na_binary_ops()[0].rhst_name);
    clear_err();

    result = inst::operation::query_pre_unary(misc::pos_type(13), "-", inst::type::BIT_VOID);
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_na_pre_unary_ops().size());
    ASSERT_EQ(misc::pos_type(13), get_na_pre_unary_ops()[0].pos);
    ASSERT_EQ("-", get_na_pre_unary_ops()[0].op_img);
    ASSERT_EQ(inst::type::BIT_VOID->name(), get_na_pre_unary_ops()[0].rhst_name);
}
