#include <vector>
#include <set>
#include <algorithm>
#include <gtest/gtest.h>

#include <misc/pos-type.h>
#include <test/phony-errors.h>

#include "test-common.h"
#include "../operation.h"
#include "../type.h"

using namespace test;

typedef ProtoTest QueryOperationTest;

static std::vector<std::string> const ARITH_BIN_OP_LIST({ "+", "-", "*", "/", "%" });
static std::vector<std::string> const COMP_OP_LIST({ "=", "<=", "<", ">=", ">", "!=" });
static std::vector<std::string> const ARITH_PRE_UNARY_OP_LIST({ "+", "-" });

namespace {

    struct IdCmp {
        bool operator()(util::sref<proto::Operation const> lhs
                      , util::sref<proto::Operation const> rhs)
        {
            return lhs.id() < rhs.id();
        }
    };

}

TEST_F(QueryOperationTest, Arithmetics)
{
    misc::position pos(1);
    std::set<util::sref<proto::Operation const>, IdCmp> op_records;
    std::for_each(ARITH_BIN_OP_LIST.begin()
                , ARITH_BIN_OP_LIST.end()
                , [&](std::string const& op)
                  {
                      util::sref<proto::Operation const> result = proto::Operation::queryBinary(
                              pos, op, proto::Type::s_int(), proto::Type::s_int());
                      ASSERT_FALSE(error::hasError()) << "operation img:" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img:" << op << " duplicate operation.";
                      ASSERT_EQ(proto::Type::s_int(), result->ret_type);
                  });
    std::for_each(ARITH_BIN_OP_LIST.begin()
                , ARITH_BIN_OP_LIST.end() - 1
                , [&](std::string const& op)
                  {
                      util::sref<proto::Operation const> result = proto::Operation::queryBinary(
                              pos, op, proto::Type::s_float(), proto::Type::s_float());
                      ASSERT_FALSE(error::hasError()) << "operation img:" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img:" << op << " duplicate operation.";
                      ASSERT_EQ(proto::Type::s_float(), result->ret_type);

                      result = proto::Operation::queryBinary(
                              pos, op, proto::Type::s_float(), proto::Type::s_int());
                      ASSERT_FALSE(error::hasError()) << "operation img:" << op << " query failed.";
                      ASSERT_EQ(proto::Type::s_float(), result->ret_type);

                      result = proto::Operation::queryBinary(
                              pos, op, proto::Type::s_int(), proto::Type::s_float());
                      ASSERT_FALSE(error::hasError()) << "operation img:" << op << " query failed.";
                      ASSERT_EQ(proto::Type::s_float(), result->ret_type);

                      result = proto::Operation::queryBinary(
                              pos, op, proto::Type::s_float(), proto::Type::s_float());
                      ASSERT_FALSE(error::hasError()) << "operation img:" << op << " query failed.";
                      ASSERT_EQ(proto::Type::s_float(), result->ret_type);
                  });

    std::for_each(ARITH_PRE_UNARY_OP_LIST.begin()
                , ARITH_PRE_UNARY_OP_LIST.end()
                , [&](std::string const& op)
                  {
                      util::sref<proto::Operation const> result(nullptr);

                      result = proto::Operation::queryPreUnary(pos, op, proto::Type::s_int());
                      ASSERT_FALSE(error::hasError()) << "operation img:" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img:" << op << " duplicate operation.";
                      ASSERT_EQ(proto::Type::s_int(), result->ret_type);

                      result = proto::Operation::queryPreUnary(pos, op, proto::Type::s_float());
                      ASSERT_FALSE(error::hasError()) << "operation img:" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img:" << op << " duplicate operation.";
                      ASSERT_EQ(proto::Type::s_float(), result->ret_type);
                  });

    std::for_each(COMP_OP_LIST.begin()
                , COMP_OP_LIST.end()
                , [&](std::string const& op)
                  {
                      util::sref<proto::Operation const> result(nullptr);

                      result = proto::Operation::queryBinary(
                              pos, op, proto::Type::s_int(), proto::Type::s_int());
                      ASSERT_FALSE(error::hasError()) << "operation img:" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img:" << op << " duplicate operation.";
                      ASSERT_EQ(proto::Type::s_bool(), result->ret_type);

                      result = proto::Operation::queryBinary(
                          pos, op, proto::Type::s_float(), proto::Type::s_float());
                      ASSERT_FALSE(error::hasError()) << "operation img:" << op << " query failed.";
                      ASSERT_TRUE(op_records.insert(result).second)
                              << "operation img:" << op << " duplicate operation.";
                      ASSERT_EQ(proto::Type::s_bool(), result->ret_type);
                  });
    ASSERT_FALSE(error::hasError());
}

TEST_F(QueryOperationTest, Booleans)
{
    misc::position pos(2);
    std::set<util::sref<proto::Operation const>, IdCmp> op_records;
    util::sref<proto::Operation const> result = proto::Operation::queryBinary(
                pos, "=", proto::Type::s_bool(), proto::Type::s_bool());
    ASSERT_FALSE(error::hasError()) << "operation img:= query failed.";
    ASSERT_TRUE(op_records.insert(result).second) << "operation img:= duplicate operation.";
    ASSERT_EQ(proto::Type::s_bool(), result->ret_type);
    result = proto::Operation::queryBinary(pos, "!=", proto::Type::s_bool(), proto::Type::s_bool());
    ASSERT_FALSE(error::hasError()) << "operation img:!= query failed.";
    ASSERT_TRUE(op_records.insert(result).second) << "operation img:!= duplicate operation.";
    ASSERT_EQ(proto::Type::s_bool(), result->ret_type);
    ASSERT_FALSE(error::hasError());
}

TEST_F(QueryOperationTest, BadInputType)
{
    misc::position pos(3);
    std::vector<std::string> all_bin_op = ARITH_BIN_OP_LIST;
    all_bin_op.insert(all_bin_op.end(), COMP_OP_LIST.begin(), COMP_OP_LIST.end());
    std::for_each(all_bin_op.begin()
                , all_bin_op.end()
                , [&](std::string const& op)
                  {
                      util::sref<proto::Operation const> result(nullptr);

                      result = proto::Operation::queryBinary(
                              pos, op, proto::Type::bad(), proto::Type::bad());
                      ASSERT_FALSE(error::hasError()) << "operation img:" << op << " query failed.";
                      ASSERT_EQ(proto::Type::bad(), result->ret_type);

                      result = proto::Operation::queryBinary(
                              pos, op, proto::Type::bad(), proto::Type::s_int());
                      ASSERT_FALSE(error::hasError()) << "operation img:" << op << " query failed.";
                      ASSERT_EQ(proto::Type::bad(), result->ret_type);

                      result = proto::Operation::queryBinary(
                              pos, op, proto::Type::s_float(), proto::Type::bad());
                      ASSERT_FALSE(error::hasError()) << "operation img:" << op << " query failed.";
                      ASSERT_EQ(proto::Type::bad(), result->ret_type);

                      result = proto::Operation::queryBinary(
                              pos, op, proto::Type::bad(), proto::Type::s_bool());
                      ASSERT_FALSE(error::hasError()) << "operation img:" << op << " query failed.";
                      ASSERT_EQ(proto::Type::bad(), result->ret_type);

                      result = proto::Operation::queryBinary(
                              pos, op, proto::Type::s_void(), proto::Type::bad());
                      ASSERT_FALSE(error::hasError()) << "operation img:" << op << " query failed.";
                      ASSERT_EQ(proto::Type::bad(), result->ret_type);
                  });

    std::for_each(ARITH_PRE_UNARY_OP_LIST.begin()
                , ARITH_PRE_UNARY_OP_LIST.end()
                , [&](std::string const& op)
                  {
                      util::sref<proto::Operation const> result = proto::Operation::queryPreUnary(
                              pos, op, proto::Type::bad());
                      ASSERT_FALSE(error::hasError()) << "operation img:" << op << " query failed.";
                      ASSERT_EQ(proto::Type::bad(), result->ret_type);
                  });
    ASSERT_FALSE(error::hasError());
}

TEST_F(QueryOperationTest, Strings)
{
    misc::position pos(4);
    std::vector<util::sref<proto::Type const>> const TYPE_LIST({
        proto::Type::s_bool(),
        proto::Type::s_int(),
        proto::Type::s_float(),
        proto::Type::s_string(),
    });
    std::for_each(TYPE_LIST.begin()
                , TYPE_LIST.end()
                , [&](util::sref<proto::Type const> t)
                  {
                      util::sref<proto::Operation const> result = proto::Operation::queryBinary(
                              pos, "+", proto::Type::s_string(), t);
                      ASSERT_FALSE(error::hasError()) << "string add type:" << t->name()
                                                      << " query failed.";
                      ASSERT_EQ(proto::Type::s_string(), result->ret_type);
                  });
    std::for_each(TYPE_LIST.begin()
                , TYPE_LIST.end()
                , [&](util::sref<proto::Type const> t)
                  {
                      util::sref<proto::Operation const> result = proto::Operation::queryBinary(
                              pos, "+", t, proto::Type::s_string());
                      ASSERT_FALSE(error::hasError()) << "type add string:" << t->name()
                                                      << " query failed.";
                      ASSERT_EQ(proto::Type::s_string(), result->ret_type);
                  });
}

TEST_F(QueryOperationTest, BadOperation)
{
    proto::Operation::queryBinary(misc::position(10)
                                , "%"
                                , proto::Type::s_float()
                                , proto::Type::s_float());
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, getNABinaryOps().size());
    ASSERT_EQ(misc::position(10), getNABinaryOps()[0].pos);
    ASSERT_EQ("%", getNABinaryOps()[0].op_img);
    ASSERT_EQ(proto::Type::s_float()->name(), getNABinaryOps()[0].rhst_name);
    clearErr();

    proto::Operation::queryBinary(misc::position(11)
                                , "<"
                                , proto::Type::s_bool()
                                , proto::Type::s_float());
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, getNABinaryOps().size());
    ASSERT_EQ(misc::position(11), getNABinaryOps()[0].pos);
    ASSERT_EQ("<", getNABinaryOps()[0].op_img);
    ASSERT_EQ(proto::Type::s_bool()->name(), getNABinaryOps()[0].lhst_name);
    ASSERT_EQ(proto::Type::s_float()->name(), getNABinaryOps()[0].rhst_name);
    clearErr();

    proto::Operation::queryBinary(misc::position(12)
                                , "+"
                                , proto::Type::s_int()
                                , proto::Type::s_bool());
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, getNABinaryOps().size());
    ASSERT_EQ(misc::position(12), getNABinaryOps()[0].pos);
    ASSERT_EQ("+", getNABinaryOps()[0].op_img);
    ASSERT_EQ(proto::Type::s_int()->name(), getNABinaryOps()[0].lhst_name);
    ASSERT_EQ(proto::Type::s_bool()->name(), getNABinaryOps()[0].rhst_name);
    clearErr();

    proto::Operation::queryPreUnary(misc::position(13), "-", proto::Type::s_void());
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, getNAPreUnaryOps().size());
    ASSERT_EQ(misc::position(13), getNAPreUnaryOps()[0].pos);
    ASSERT_EQ("-", getNAPreUnaryOps()[0].op_img);
    ASSERT_EQ(proto::Type::s_void()->name(), getNAPreUnaryOps()[0].rhst_name);
}
