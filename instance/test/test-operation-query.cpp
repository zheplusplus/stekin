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
        clearErr();
    }
};

std::vector<std::string> const ARITH_BIN_OP_LIST({ "+", "-", "*", "/", "%" });
std::vector<std::string> const COMP_OP_LIST({ "=", "<=", "<", ">=", ">", "!=" });
std::vector<std::string> const ARITH_PRE_UNARY_OP_LIST({ "+", "-" });

TEST_F(QueryOperationTest, NoError)
{
    int lineno = 1;
    std::set<inst::Operation const*> op_records;
    std::for_each(ARITH_BIN_OP_LIST.begin()
                , ARITH_BIN_OP_LIST.end()
                , [&](std::string const& op)
                  {
                      inst::Operation const* result = inst::Operation::queryBinary(
                              misc::position(lineno), op, inst::Type::BIT_INT, inst::Type::BIT_INT);
                      ASSERT_FALSE(error::hasError()) << "operation img=" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img=" << op << " duplicate operation.";
                      ASSERT_EQ(inst::Type::BIT_INT, result->ret_type);
                      ++lineno;
                  });

    std::for_each(ARITH_BIN_OP_LIST.begin()
                , ARITH_BIN_OP_LIST.end() - 1
                , [&](std::string const& op)
                  {
                      inst::Operation const* result = inst::Operation::queryBinary(
                              misc::position(lineno), op, inst::Type::BIT_FLOAT, inst::Type::BIT_FLOAT);
                      ASSERT_FALSE(error::hasError()) << "operation img=" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img=" << op << " duplicate operation.";
                      ASSERT_EQ(inst::Type::BIT_FLOAT, result->ret_type);
                      ++lineno;
                  });

    std::for_each(ARITH_PRE_UNARY_OP_LIST.begin()
                , ARITH_PRE_UNARY_OP_LIST.end()
                , [&](std::string const& op)
                  {
                      inst::Operation const* result;

                      result = inst::Operation::queryPreUnary(
                              misc::position(lineno), op, inst::Type::BIT_INT);
                      ASSERT_FALSE(error::hasError()) << "operation img=" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img=" << op << " duplicate operation.";
                      ASSERT_EQ(inst::Type::BIT_INT, result->ret_type);
                      ++lineno;

                      result = inst::Operation::queryPreUnary(
                              misc::position(lineno), op, inst::Type::BIT_FLOAT);
                      ASSERT_FALSE(error::hasError()) << "operation img=" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img=" << op << " duplicate operation.";
                      ASSERT_EQ(inst::Type::BIT_FLOAT, result->ret_type);
                      ++lineno;
                  });

    std::for_each(COMP_OP_LIST.begin()
                , COMP_OP_LIST.end()
                , [&](std::string const& op)
                  {
                      inst::Operation const* result;

                      result = inst::Operation::queryBinary(
                              misc::position(lineno), op, inst::Type::BIT_INT, inst::Type::BIT_INT);
                      ASSERT_FALSE(error::hasError()) << "operation img=" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img=" << op << " duplicate operation.";
                      ASSERT_EQ(inst::Type::BIT_BOOL, result->ret_type);
                      ++lineno;

                      result = inst::Operation::queryBinary(
                              misc::position(lineno), op, inst::Type::BIT_FLOAT, inst::Type::BIT_FLOAT);
                      ASSERT_FALSE(error::hasError()) << "operation img=" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img=" << op << " duplicate operation.";
                      ASSERT_EQ(inst::Type::BIT_BOOL, result->ret_type);
                      ++lineno;
                  });
    ASSERT_FALSE(error::hasError());
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
                      inst::Operation const* result;

                      result = inst::Operation::queryBinary(
                            misc::position(lineno), op, inst::Type::BAD_TYPE, inst::Type::BAD_TYPE);
                      ASSERT_FALSE(error::hasError()) << "operation img=" << op << " query failed.";
                      ASSERT_EQ(inst::Type::BAD_TYPE, result->ret_type);
                      ++lineno;

                      result = inst::Operation::queryBinary(
                            misc::position(lineno), op, inst::Type::BAD_TYPE, inst::Type::BIT_INT);
                      ASSERT_FALSE(error::hasError()) << "operation img=" << op << " query failed.";
                      ASSERT_EQ(inst::Type::BAD_TYPE, result->ret_type);
                      ++lineno;

                      result = inst::Operation::queryBinary(
                          misc::position(lineno), op, inst::Type::BIT_FLOAT, inst::Type::BAD_TYPE);
                      ASSERT_FALSE(error::hasError()) << "operation img=" << op << " query failed.";
                      ASSERT_EQ(inst::Type::BAD_TYPE, result->ret_type);
                      ++lineno;

                      result = inst::Operation::queryBinary(
                            misc::position(lineno), op, inst::Type::BAD_TYPE, inst::Type::BIT_BOOL);
                      ASSERT_FALSE(error::hasError()) << "operation img=" << op << " query failed.";
                      ASSERT_EQ(inst::Type::BAD_TYPE, result->ret_type);
                      ++lineno;

                      result = inst::Operation::queryBinary(
                            misc::position(lineno), op, inst::Type::BIT_VOID, inst::Type::BAD_TYPE);
                      ASSERT_FALSE(error::hasError()) << "operation img=" << op << " query failed.";
                      ASSERT_EQ(inst::Type::BAD_TYPE, result->ret_type);
                      ++lineno;
                  });

    std::for_each(ARITH_PRE_UNARY_OP_LIST.begin()
                , ARITH_PRE_UNARY_OP_LIST.end()
                , [&](std::string const& op)
                  {
                      inst::Operation const* result = inst::Operation::queryPreUnary(
                              misc::position(lineno), op, inst::Type::BAD_TYPE);
                      ASSERT_FALSE(error::hasError()) << "operation img=" << op << " query failed.";
                      ASSERT_EQ(inst::Type::BAD_TYPE, result->ret_type);
                      ++lineno;
                  });
    ASSERT_FALSE(error::hasError());
}

TEST_F(QueryOperationTest, BadOperation)
{
    inst::Operation::queryBinary(misc::position(10)
                               , "%"
                               , inst::Type::BIT_FLOAT
                               , inst::Type::BIT_FLOAT);
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, getNABinaryOps().size());
    ASSERT_EQ(misc::position(10), getNABinaryOps()[0].pos);
    ASSERT_EQ("%", getNABinaryOps()[0].op_img);
    ASSERT_EQ(inst::Type::BIT_FLOAT->name(), getNABinaryOps()[0].rhst_name);
    clearErr();

    inst::Operation::queryBinary(misc::position(11)
                               , "<"
                               , inst::Type::BIT_INT
                               , inst::Type::BIT_FLOAT);
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, getNABinaryOps().size());
    ASSERT_EQ(misc::position(11), getNABinaryOps()[0].pos);
    ASSERT_EQ("<", getNABinaryOps()[0].op_img);
    ASSERT_EQ(inst::Type::BIT_INT->name(), getNABinaryOps()[0].lhst_name);
    ASSERT_EQ(inst::Type::BIT_FLOAT->name(), getNABinaryOps()[0].rhst_name);
    clearErr();

    inst::Operation::queryBinary(misc::position(12)
                               , "+"
                               , inst::Type::BIT_INT
                               , inst::Type::BIT_BOOL);
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, getNABinaryOps().size());
    ASSERT_EQ(misc::position(12), getNABinaryOps()[0].pos);
    ASSERT_EQ("+", getNABinaryOps()[0].op_img);
    ASSERT_EQ(inst::Type::BIT_INT->name(), getNABinaryOps()[0].lhst_name);
    ASSERT_EQ(inst::Type::BIT_BOOL->name(), getNABinaryOps()[0].rhst_name);
    clearErr();

    inst::Operation::queryPreUnary(misc::position(13), "-", inst::Type::BIT_VOID);
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, getNAPreUnaryOps().size());
    ASSERT_EQ(misc::position(13), getNAPreUnaryOps()[0].pos);
    ASSERT_EQ("-", getNAPreUnaryOps()[0].op_img);
    ASSERT_EQ(inst::Type::BIT_VOID->name(), getNAPreUnaryOps()[0].rhst_name);
}
