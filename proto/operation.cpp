#include <map>

#include "operation.h"
#include "../instance/type.h"
#include "../report/errors.h"

using namespace proto;

static Operation const BIT_INT_ADD(inst::Type::BIT_INT, "+");
static Operation const BIT_INT_SUB(inst::Type::BIT_INT, "-");
static Operation const BIT_INT_MUL(inst::Type::BIT_INT, "*");
static Operation const BIT_INT_DIV(inst::Type::BIT_INT, "/");
static Operation const BIT_INT_MOD(inst::Type::BIT_INT, "%");

static Operation const BIT_FLOAT_ADD(inst::Type::BIT_FLOAT, "+");
static Operation const BIT_FLOAT_SUB(inst::Type::BIT_FLOAT, "-");
static Operation const BIT_FLOAT_MUL(inst::Type::BIT_FLOAT, "*");
static Operation const BIT_FLOAT_DIV(inst::Type::BIT_FLOAT, "/");

static Operation const BIT_BOOL_EQ(inst::Type::BIT_BOOL, "==");
static Operation const BIT_BOOL_NE(inst::Type::BIT_BOOL, "!=");

static Operation const BIT_INT_EQ(inst::Type::BIT_BOOL, "==");
static Operation const BIT_INT_LE(inst::Type::BIT_BOOL, "<=");
static Operation const BIT_INT_LT(inst::Type::BIT_BOOL, "<");
static Operation const BIT_INT_GE(inst::Type::BIT_BOOL, ">=");
static Operation const BIT_INT_GT(inst::Type::BIT_BOOL, ">");
static Operation const BIT_INT_NE(inst::Type::BIT_BOOL, "!=");

static Operation const BIT_FLOAT_EQ(inst::Type::BIT_BOOL, "==");
static Operation const BIT_FLOAT_LE(inst::Type::BIT_BOOL, "<=");
static Operation const BIT_FLOAT_LT(inst::Type::BIT_BOOL, "<");
static Operation const BIT_FLOAT_GE(inst::Type::BIT_BOOL, ">=");
static Operation const BIT_FLOAT_GT(inst::Type::BIT_BOOL, ">");
static Operation const BIT_FLOAT_NE(inst::Type::BIT_BOOL, "!=");

static Operation const BIT_POSI_INT(inst::Type::BIT_INT, "+");
static Operation const BIT_POSI_FLOAT(inst::Type::BIT_FLOAT, "+");

static Operation const BIT_NEGA_INT(inst::Type::BIT_INT, "-");
static Operation const BIT_NEGA_FLOAT(inst::Type::BIT_FLOAT, "-");

static Operation const BAD_OPERATION(inst::Type::BAD_TYPE, "");

namespace {

    struct BadOpException {};

    struct BinaryOpMap {
        static BinaryOpMap const& instance()
        {
            static BinaryOpMap inst;
            return inst;
        }

        Operation const* query(std::string const& op
                             , util::sref<inst::Type const> lhs
                             , util::sref<inst::Type const> rhs) const
        {
            if (inst::Type::BAD_TYPE == lhs || inst::Type::BAD_TYPE == rhs) {
                return &BAD_OPERATION;
            }

            auto first_stage = _map.find(op);
            if (_map.end() == first_stage) {
                throw BadOpException();
            }

            auto second_stage = first_stage->second.find(lhs);
            if (first_stage->second.end() == second_stage) {
                throw BadOpException();
            }

            auto third_stage = second_stage->second.find(rhs);
            if (second_stage->second.end() == third_stage) {
                throw BadOpException();
            }

            return third_stage->second;
        }
    private:
        BinaryOpMap& add(std::string const& op_id
                       , util::sref<inst::Type const> lhs
                       , util::sref<inst::Type const> rhs
                       , Operation const* oper)
        {
            _map[op_id][lhs][rhs] = oper;
            return *this;
        }
    private:
        BinaryOpMap()
        {
            (*this)
                .add("+", inst::Type::BIT_INT, inst::Type::BIT_INT, &BIT_INT_ADD)
                .add("-", inst::Type::BIT_INT, inst::Type::BIT_INT, &BIT_INT_SUB)
                .add("*", inst::Type::BIT_INT, inst::Type::BIT_INT, &BIT_INT_MUL)
                .add("/", inst::Type::BIT_INT, inst::Type::BIT_INT, &BIT_INT_DIV)
                .add("%", inst::Type::BIT_INT, inst::Type::BIT_INT, &BIT_INT_MOD)

                .add("+", inst::Type::BIT_FLOAT, inst::Type::BIT_FLOAT, &BIT_FLOAT_ADD)
                .add("-", inst::Type::BIT_FLOAT, inst::Type::BIT_FLOAT, &BIT_FLOAT_SUB)
                .add("*", inst::Type::BIT_FLOAT, inst::Type::BIT_FLOAT, &BIT_FLOAT_MUL)
                .add("/", inst::Type::BIT_FLOAT, inst::Type::BIT_FLOAT, &BIT_FLOAT_DIV)

                .add("=", inst::Type::BIT_BOOL, inst::Type::BIT_BOOL, &BIT_BOOL_EQ)
                .add("!=", inst::Type::BIT_BOOL, inst::Type::BIT_BOOL, &BIT_BOOL_NE)

                .add("+", inst::Type::BIT_INT, inst::Type::BIT_FLOAT, &BIT_FLOAT_ADD)
                .add("-", inst::Type::BIT_INT, inst::Type::BIT_FLOAT, &BIT_FLOAT_SUB)
                .add("*", inst::Type::BIT_INT, inst::Type::BIT_FLOAT, &BIT_FLOAT_MUL)
                .add("/", inst::Type::BIT_INT, inst::Type::BIT_FLOAT, &BIT_FLOAT_DIV)

                .add("+", inst::Type::BIT_FLOAT, inst::Type::BIT_INT, &BIT_FLOAT_ADD)
                .add("-", inst::Type::BIT_FLOAT, inst::Type::BIT_INT, &BIT_FLOAT_SUB)
                .add("*", inst::Type::BIT_FLOAT, inst::Type::BIT_INT, &BIT_FLOAT_MUL)
                .add("/", inst::Type::BIT_FLOAT, inst::Type::BIT_INT, &BIT_FLOAT_DIV)

                .add("=", inst::Type::BIT_INT, inst::Type::BIT_INT, &BIT_INT_EQ)
                .add("<=", inst::Type::BIT_INT, inst::Type::BIT_INT, &BIT_INT_LE)
                .add("<", inst::Type::BIT_INT, inst::Type::BIT_INT, &BIT_INT_LT)
                .add(">=", inst::Type::BIT_INT, inst::Type::BIT_INT, &BIT_INT_GE)
                .add(">", inst::Type::BIT_INT, inst::Type::BIT_INT, &BIT_INT_GT)
                .add("!=", inst::Type::BIT_INT, inst::Type::BIT_INT, &BIT_INT_NE)

                .add("=", inst::Type::BIT_FLOAT, inst::Type::BIT_FLOAT, &BIT_FLOAT_EQ)
                .add("<=", inst::Type::BIT_FLOAT, inst::Type::BIT_FLOAT, &BIT_FLOAT_LE)
                .add("<", inst::Type::BIT_FLOAT, inst::Type::BIT_FLOAT, &BIT_FLOAT_LT)
                .add(">=", inst::Type::BIT_FLOAT, inst::Type::BIT_FLOAT, &BIT_FLOAT_GE)
                .add(">", inst::Type::BIT_FLOAT, inst::Type::BIT_FLOAT, &BIT_FLOAT_GT)
                .add("!=", inst::Type::BIT_FLOAT, inst::Type::BIT_FLOAT, &BIT_FLOAT_NE)

                .add("=", inst::Type::BIT_INT, inst::Type::BIT_FLOAT, &BIT_FLOAT_EQ)
                .add("<=", inst::Type::BIT_INT, inst::Type::BIT_FLOAT, &BIT_FLOAT_LE)
                .add("<", inst::Type::BIT_INT, inst::Type::BIT_FLOAT, &BIT_FLOAT_LT)
                .add(">=", inst::Type::BIT_INT, inst::Type::BIT_FLOAT, &BIT_FLOAT_GE)
                .add(">", inst::Type::BIT_INT, inst::Type::BIT_FLOAT, &BIT_FLOAT_GT)
                .add("!=", inst::Type::BIT_INT, inst::Type::BIT_FLOAT, &BIT_FLOAT_NE)

                .add("=", inst::Type::BIT_FLOAT, inst::Type::BIT_INT, &BIT_FLOAT_EQ)
                .add("<=", inst::Type::BIT_FLOAT, inst::Type::BIT_INT, &BIT_FLOAT_LE)
                .add("<", inst::Type::BIT_FLOAT, inst::Type::BIT_INT, &BIT_FLOAT_LT)
                .add(">=", inst::Type::BIT_FLOAT, inst::Type::BIT_INT, &BIT_FLOAT_GE)
                .add(">", inst::Type::BIT_FLOAT, inst::Type::BIT_INT, &BIT_FLOAT_GT)
                .add("!=", inst::Type::BIT_FLOAT, inst::Type::BIT_INT, &BIT_FLOAT_NE)
            ;
        }

        std::map<std::string
               , std::map<util::sref<inst::Type const>
                        , std::map<util::sref<inst::Type const>
                                 , Operation const*>>> _map;
    };

    struct PreUnaryOpMap {
        static PreUnaryOpMap const& instance()
        {
            static PreUnaryOpMap inst;
            return inst;
        }

        Operation const* query(std::string const& op, util::sref<inst::Type const> rhs) const
        {
            if (inst::Type::BAD_TYPE == rhs) {
                return &BAD_OPERATION;
            }

            auto first_stage = _map.find(op);
            if (_map.end() == first_stage) {
                throw BadOpException();
            }

            auto second_stage = first_stage->second.find(rhs);
            if (first_stage->second.end() == second_stage) {
                throw BadOpException();
            }

            return second_stage->second;
        }
    private:
        PreUnaryOpMap& add(std::string const& op_id
                         , util::sref<inst::Type const> rhs
                         , Operation const* oper)
        {
            _map[op_id][rhs] = oper;
            return *this;
        }
    private:
        PreUnaryOpMap()
        {
            (*this)
                .add("+", inst::Type::BIT_INT, &BIT_POSI_INT)
                .add("-", inst::Type::BIT_INT, &BIT_NEGA_INT)

                .add("+", inst::Type::BIT_FLOAT, &BIT_POSI_FLOAT)
                .add("-", inst::Type::BIT_FLOAT, &BIT_NEGA_FLOAT)
            ;
        }

        std::map<std::string, std::map<util::sref<inst::Type const>, Operation const*>> _map;
    };

}

Operation const* Operation::queryBinary(misc::position const& pos
                                      , std::string const& op
                                      , util::sref<inst::Type const> lhs
                                      , util::sref<inst::Type const> rhs)
{
    try {
        return BinaryOpMap::instance().query(op, lhs, rhs);
    } catch (BadOpException) {
        error::binaryOpNotAvai(pos, op, lhs->name(), rhs->name());
        return &BAD_OPERATION;
    }
}

Operation const* Operation::queryPreUnary(misc::position const& pos
                                        , std::string const& op
                                        , util::sref<inst::Type const> rhs)
{
    try {
        return PreUnaryOpMap::instance().query(op, rhs);
    } catch (BadOpException) {
        error::preUnaryOpNotAvai(pos, op, rhs->name());
        return &BAD_OPERATION;
    }
}
