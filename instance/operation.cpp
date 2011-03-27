#include <map>

#include "operation.h"
#include "type.h"
#include "../report/errors.h"

using namespace inst;

static operation const BIT_INT_ADD(type::BIT_INT, "+");
static operation const BIT_INT_SUB(type::BIT_INT, "-");
static operation const BIT_INT_MUL(type::BIT_INT, "*");
static operation const BIT_INT_DIV(type::BIT_INT, "/");
static operation const BIT_INT_MOD(type::BIT_INT, "%");

static operation const BIT_FLOAT_ADD(type::BIT_FLOAT, "+");
static operation const BIT_FLOAT_SUB(type::BIT_FLOAT, "-");
static operation const BIT_FLOAT_MUL(type::BIT_FLOAT, "*");
static operation const BIT_FLOAT_DIV(type::BIT_FLOAT, "/");

static operation const BIT_INT_EQ(type::BIT_BOOL, "==");
static operation const BIT_INT_LE(type::BIT_BOOL, "<=");
static operation const BIT_INT_LT(type::BIT_BOOL, "<");
static operation const BIT_INT_GE(type::BIT_BOOL, ">=");
static operation const BIT_INT_GT(type::BIT_BOOL, ">");
static operation const BIT_INT_NE(type::BIT_BOOL, "!=");

static operation const BIT_FLOAT_EQ(type::BIT_BOOL, "==");
static operation const BIT_FLOAT_LE(type::BIT_BOOL, "<=");
static operation const BIT_FLOAT_LT(type::BIT_BOOL, "<");
static operation const BIT_FLOAT_GE(type::BIT_BOOL, ">=");
static operation const BIT_FLOAT_GT(type::BIT_BOOL, ">");
static operation const BIT_FLOAT_NE(type::BIT_BOOL, "!=");

static operation const BIT_POSI_INT(type::BIT_INT, "+");
static operation const BIT_POSI_FLOAT(type::BIT_FLOAT, "+");

static operation const BIT_NEGA_INT(type::BIT_INT, "-");
static operation const BIT_NEGA_FLOAT(type::BIT_FLOAT, "-");

static operation const BAD_OPERATION(type::BAD_TYPE, "");

namespace {

    struct bad_op_exception {};

    struct binary_op_map {
        static binary_op_map const& instance()
        {
            static binary_op_map inst;
            return inst;
        }

        operation const* query(std::string const& op, util::sref<type const> lhs, util::sref<type const> rhs) const
        {
            if (type::BAD_TYPE == lhs || type::BAD_TYPE == rhs) {
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
        binary_op_map& add(std::string const& op_id, util::sref<type const> lhs, util::sref<type const> rhs, operation const* oper)
        {
            _map[op_id][lhs][rhs] = oper;
            return *this;
        }
    private:
        binary_op_map()
        {
            (*this)
                .add("+", type::BIT_INT, type::BIT_INT, &BIT_INT_ADD)
                .add("-", type::BIT_INT, type::BIT_INT, &BIT_INT_SUB)
                .add("*", type::BIT_INT, type::BIT_INT, &BIT_INT_MUL)
                .add("/", type::BIT_INT, type::BIT_INT, &BIT_INT_DIV)
                .add("%", type::BIT_INT, type::BIT_INT, &BIT_INT_MOD)

                .add("+", type::BIT_FLOAT, type::BIT_FLOAT, &BIT_FLOAT_ADD)
                .add("-", type::BIT_FLOAT, type::BIT_FLOAT, &BIT_FLOAT_SUB)
                .add("*", type::BIT_FLOAT, type::BIT_FLOAT, &BIT_FLOAT_MUL)
                .add("/", type::BIT_FLOAT, type::BIT_FLOAT, &BIT_FLOAT_DIV)

                .add("=", type::BIT_INT, type::BIT_INT, &BIT_INT_EQ)
                .add("<=", type::BIT_INT, type::BIT_INT, &BIT_INT_LE)
                .add("<", type::BIT_INT, type::BIT_INT, &BIT_INT_LT)
                .add(">=", type::BIT_INT, type::BIT_INT, &BIT_INT_GE)
                .add(">", type::BIT_INT, type::BIT_INT, &BIT_INT_GT)
                .add("!=", type::BIT_INT, type::BIT_INT, &BIT_INT_NE)

                .add("=", type::BIT_FLOAT, type::BIT_FLOAT, &BIT_FLOAT_EQ)
                .add("<=", type::BIT_FLOAT, type::BIT_FLOAT, &BIT_FLOAT_LE)
                .add("<", type::BIT_FLOAT, type::BIT_FLOAT, &BIT_FLOAT_LT)
                .add(">=", type::BIT_FLOAT, type::BIT_FLOAT, &BIT_FLOAT_GE)
                .add(">", type::BIT_FLOAT, type::BIT_FLOAT, &BIT_FLOAT_GT)
                .add("!=", type::BIT_FLOAT, type::BIT_FLOAT, &BIT_FLOAT_NE)
            ;
        }

        std::map<std::string, std::map<util::sref<type const>, std::map<util::sref<type const>, operation const*>>> _map;
    };

    struct pre_unary_op_map {
        static pre_unary_op_map const& instance()
        {
            static pre_unary_op_map inst;
            return inst;
        }

        operation const* query(std::string const& op, util::sref<type const> rhs) const
        {
            if (type::BAD_TYPE == rhs) {
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
        pre_unary_op_map& add(std::string const& op_id, util::sref<type const> rhs, operation const* oper)
        {
            _map[op_id][rhs] = oper;
            return *this;
        }
    private:
        pre_unary_op_map()
        {
            (*this)
                .add("+", type::BIT_INT, &BIT_POSI_INT)
                .add("-", type::BIT_INT, &BIT_NEGA_INT)

                .add("+", type::BIT_FLOAT, &BIT_POSI_FLOAT)
                .add("-", type::BIT_FLOAT, &BIT_NEGA_FLOAT)
            ;
        }

        std::map<std::string, std::map<util::sref<type const>, operation const*>> _map;
    };

}

operation const* operation::query_binary(misc::pos_type const& pos
                                       , std::string const& op
                                       , util::sref<type const> lhs
                                       , util::sref<type const> rhs)
{
    try {
        return binary_op_map::instance().query(op, lhs, rhs);
    } catch (bad_op_exception) {
        error::binary_op_not_avai(pos, op, lhs->name(), rhs->name());
        return &BAD_OPERATION;
    }
}

operation const* operation::query_pre_unary(misc::pos_type const& pos
                                          , std::string const& op
                                          , util::sref<type const> rhs)
{
    try {
        return pre_unary_op_map::instance().query(op, rhs);
    } catch (bad_op_exception) {
        error::pre_unary_op_not_avai(pos, op, rhs->name());
        return &BAD_OPERATION;
    }
}
