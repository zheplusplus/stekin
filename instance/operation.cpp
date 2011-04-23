#include <map>

#include "operation.h"
#include "type.h"
#include "../report/errors.h"

using namespace inst;

static operation const BIT_INT_ADD(Type::BIT_INT, "+");
static operation const BIT_INT_SUB(Type::BIT_INT, "-");
static operation const BIT_INT_MUL(Type::BIT_INT, "*");
static operation const BIT_INT_DIV(Type::BIT_INT, "/");
static operation const BIT_INT_MOD(Type::BIT_INT, "%");

static operation const BIT_FLOAT_ADD(Type::BIT_FLOAT, "+");
static operation const BIT_FLOAT_SUB(Type::BIT_FLOAT, "-");
static operation const BIT_FLOAT_MUL(Type::BIT_FLOAT, "*");
static operation const BIT_FLOAT_DIV(Type::BIT_FLOAT, "/");

static operation const BIT_INT_EQ(Type::BIT_BOOL, "==");
static operation const BIT_INT_LE(Type::BIT_BOOL, "<=");
static operation const BIT_INT_LT(Type::BIT_BOOL, "<");
static operation const BIT_INT_GE(Type::BIT_BOOL, ">=");
static operation const BIT_INT_GT(Type::BIT_BOOL, ">");
static operation const BIT_INT_NE(Type::BIT_BOOL, "!=");

static operation const BIT_FLOAT_EQ(Type::BIT_BOOL, "==");
static operation const BIT_FLOAT_LE(Type::BIT_BOOL, "<=");
static operation const BIT_FLOAT_LT(Type::BIT_BOOL, "<");
static operation const BIT_FLOAT_GE(Type::BIT_BOOL, ">=");
static operation const BIT_FLOAT_GT(Type::BIT_BOOL, ">");
static operation const BIT_FLOAT_NE(Type::BIT_BOOL, "!=");

static operation const BIT_POSI_INT(Type::BIT_INT, "+");
static operation const BIT_POSI_FLOAT(Type::BIT_FLOAT, "+");

static operation const BIT_NEGA_INT(Type::BIT_INT, "-");
static operation const BIT_NEGA_FLOAT(Type::BIT_FLOAT, "-");

static operation const BAD_OPERATION(Type::BAD_TYPE, "");

namespace {

    struct bad_op_exception {};

    struct BinaryOp_map {
        static BinaryOp_map const& instance()
        {
            static BinaryOp_map inst;
            return inst;
        }

        operation const* query(std::string const& op, util::sref<Type const> lhs, util::sref<Type const> rhs) const
        {
            if (Type::BAD_TYPE == lhs || Type::BAD_TYPE == rhs) {
                return &BAD_OPERATION;
            }

            auto first_stage = _map.find(op);
            if (_map.end() == first_stage) {
                throw bad_op_exception();
            }

            auto second_stage = first_stage->second.find(lhs);
            if (first_stage->second.end() == second_stage) {
                throw bad_op_exception();
            }

            auto third_stage = second_stage->second.find(rhs);
            if (second_stage->second.end() == third_stage) {
                throw bad_op_exception();
            }

            return third_stage->second;
        }
    private:
        BinaryOp_map& add(std::string const& op_id, util::sref<Type const> lhs, util::sref<Type const> rhs, operation const* oper)
        {
            _map[op_id][lhs][rhs] = oper;
            return *this;
        }
    private:
        BinaryOp_map()
        {
            (*this)
                .add("+", Type::BIT_INT, Type::BIT_INT, &BIT_INT_ADD)
                .add("-", Type::BIT_INT, Type::BIT_INT, &BIT_INT_SUB)
                .add("*", Type::BIT_INT, Type::BIT_INT, &BIT_INT_MUL)
                .add("/", Type::BIT_INT, Type::BIT_INT, &BIT_INT_DIV)
                .add("%", Type::BIT_INT, Type::BIT_INT, &BIT_INT_MOD)

                .add("+", Type::BIT_FLOAT, Type::BIT_FLOAT, &BIT_FLOAT_ADD)
                .add("-", Type::BIT_FLOAT, Type::BIT_FLOAT, &BIT_FLOAT_SUB)
                .add("*", Type::BIT_FLOAT, Type::BIT_FLOAT, &BIT_FLOAT_MUL)
                .add("/", Type::BIT_FLOAT, Type::BIT_FLOAT, &BIT_FLOAT_DIV)

                .add("=", Type::BIT_INT, Type::BIT_INT, &BIT_INT_EQ)
                .add("<=", Type::BIT_INT, Type::BIT_INT, &BIT_INT_LE)
                .add("<", Type::BIT_INT, Type::BIT_INT, &BIT_INT_LT)
                .add(">=", Type::BIT_INT, Type::BIT_INT, &BIT_INT_GE)
                .add(">", Type::BIT_INT, Type::BIT_INT, &BIT_INT_GT)
                .add("!=", Type::BIT_INT, Type::BIT_INT, &BIT_INT_NE)

                .add("=", Type::BIT_FLOAT, Type::BIT_FLOAT, &BIT_FLOAT_EQ)
                .add("<=", Type::BIT_FLOAT, Type::BIT_FLOAT, &BIT_FLOAT_LE)
                .add("<", Type::BIT_FLOAT, Type::BIT_FLOAT, &BIT_FLOAT_LT)
                .add(">=", Type::BIT_FLOAT, Type::BIT_FLOAT, &BIT_FLOAT_GE)
                .add(">", Type::BIT_FLOAT, Type::BIT_FLOAT, &BIT_FLOAT_GT)
                .add("!=", Type::BIT_FLOAT, Type::BIT_FLOAT, &BIT_FLOAT_NE)
            ;
        }

        std::map<std::string, std::map<util::sref<Type const>, std::map<util::sref<Type const>, operation const*>>> _map;
    };

    struct PreUnaryOp_map {
        static PreUnaryOp_map const& instance()
        {
            static PreUnaryOp_map inst;
            return inst;
        }

        operation const* query(std::string const& op, util::sref<Type const> rhs) const
        {
            if (Type::BAD_TYPE == rhs) {
                return &BAD_OPERATION;
            }

            auto first_stage = _map.find(op);
            if (_map.end() == first_stage) {
                throw bad_op_exception();
            }

            auto second_stage = first_stage->second.find(rhs);
            if (first_stage->second.end() == second_stage) {
                throw bad_op_exception();
            }

            return second_stage->second;
        }
    private:
        PreUnaryOp_map& add(std::string const& op_id, util::sref<Type const> rhs, operation const* oper)
        {
            _map[op_id][rhs] = oper;
            return *this;
        }
    private:
        PreUnaryOp_map()
        {
            (*this)
                .add("+", Type::BIT_INT, &BIT_POSI_INT)
                .add("-", Type::BIT_INT, &BIT_NEGA_INT)

                .add("+", Type::BIT_FLOAT, &BIT_POSI_FLOAT)
                .add("-", Type::BIT_FLOAT, &BIT_NEGA_FLOAT)
            ;
        }

        std::map<std::string, std::map<util::sref<Type const>, operation const*>> _map;
    };

}

operation const* operation::queryBinary(misc::position const& pos
                                       , std::string const& op
                                       , util::sref<Type const> lhs
                                       , util::sref<Type const> rhs)
{
    try {
        return BinaryOp_map::instance().query(op, lhs, rhs);
    } catch (bad_op_exception) {
        error::binaryOpNotAvai(pos, op, lhs->name(), rhs->name());
        return &BAD_OPERATION;
    }
}

operation const* operation::queryPreUnary(misc::position const& pos
                                          , std::string const& op
                                          , util::sref<Type const> rhs)
{
    try {
        return PreUnaryOp_map::instance().query(op, rhs);
    } catch (bad_op_exception) {
        error::preUnaryOpNotAvai(pos, op, rhs->name());
        return &BAD_OPERATION;
    }
}
