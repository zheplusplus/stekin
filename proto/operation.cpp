#include <map>

#include <report/errors.h>

#include "operation.h"
#include "type.h"

using namespace proto;

static Operation const BIT_INT_ADD(Type::s_int(), "+");
static Operation const BIT_INT_SUB(Type::s_int(), "-");
static Operation const BIT_INT_MUL(Type::s_int(), "*");
static Operation const BIT_INT_DIV(Type::s_int(), "/");
static Operation const BIT_INT_MOD(Type::s_int(), "%");

static Operation const BIT_FLOAT_ADD(Type::s_float(), "+");
static Operation const BIT_FLOAT_SUB(Type::s_float(), "-");
static Operation const BIT_FLOAT_MUL(Type::s_float(), "*");
static Operation const BIT_FLOAT_DIV(Type::s_float(), "/");

static Operation const BIT_BOOL_EQ(Type::s_bool(), "==");
static Operation const BIT_BOOL_NE(Type::s_bool(), "!=");

static Operation const BIT_INT_EQ(Type::s_bool(), "==");
static Operation const BIT_INT_LE(Type::s_bool(), "<=");
static Operation const BIT_INT_LT(Type::s_bool(), "<");
static Operation const BIT_INT_GE(Type::s_bool(), ">=");
static Operation const BIT_INT_GT(Type::s_bool(), ">");
static Operation const BIT_INT_NE(Type::s_bool(), "!=");

static Operation const BIT_FLOAT_EQ(Type::s_bool(), "==");
static Operation const BIT_FLOAT_LE(Type::s_bool(), "<=");
static Operation const BIT_FLOAT_LT(Type::s_bool(), "<");
static Operation const BIT_FLOAT_GE(Type::s_bool(), ">=");
static Operation const BIT_FLOAT_GT(Type::s_bool(), ">");
static Operation const BIT_FLOAT_NE(Type::s_bool(), "!=");

static Operation const BIT_STRING_ADD(Type::s_string(), "+");

static Operation const BIT_POSI_INT(Type::s_int(), "+");
static Operation const BIT_POSI_FLOAT(Type::s_float(), "+");

static Operation const BIT_NEGA_INT(Type::s_int(), "-");
static Operation const BIT_NEGA_FLOAT(Type::s_float(), "-");

static Operation const BAD_OPERATION(Type::bad(), "");

namespace {

    struct BadOpException {};

    struct BinaryOpMap {
        static BinaryOpMap const& instance()
        {
            static BinaryOpMap inst;
            return inst;
        }

        util::sref<Operation const> query(std::string const& op
                                        , util::sref<Type const> lhs
                                        , util::sref<Type const> rhs) const
        {
            if (Type::bad() == lhs || Type::bad() == rhs) {
                return util::mkref(BAD_OPERATION);
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
        BinaryOpMap& _add(std::string const& op_id
                        , util::sref<Type const> lhs
                        , util::sref<Type const> rhs
                        , util::sref<Operation const> oper)
        {
            _map[op_id][lhs].insert(std::make_pair(rhs, oper));
            return *this;
        }
    private:
        BinaryOpMap()
        {
            (*this)
                ._add("+", Type::s_int(), Type::s_int(), util::mkref(BIT_INT_ADD))
                ._add("-", Type::s_int(), Type::s_int(), util::mkref(BIT_INT_SUB))
                ._add("*", Type::s_int(), Type::s_int(), util::mkref(BIT_INT_MUL))
                ._add("/", Type::s_int(), Type::s_int(), util::mkref(BIT_INT_DIV))
                ._add("%", Type::s_int(), Type::s_int(), util::mkref(BIT_INT_MOD))

                ._add("+", Type::s_float(), Type::s_float(), util::mkref(BIT_FLOAT_ADD))
                ._add("-", Type::s_float(), Type::s_float(), util::mkref(BIT_FLOAT_SUB))
                ._add("*", Type::s_float(), Type::s_float(), util::mkref(BIT_FLOAT_MUL))
                ._add("/", Type::s_float(), Type::s_float(), util::mkref(BIT_FLOAT_DIV))

                ._add("=", Type::s_bool(), Type::s_bool(), util::mkref(BIT_BOOL_EQ))
                ._add("!=", Type::s_bool(), Type::s_bool(), util::mkref(BIT_BOOL_NE))

                ._add("+", Type::s_int(), Type::s_float(), util::mkref(BIT_FLOAT_ADD))
                ._add("-", Type::s_int(), Type::s_float(), util::mkref(BIT_FLOAT_SUB))
                ._add("*", Type::s_int(), Type::s_float(), util::mkref(BIT_FLOAT_MUL))
                ._add("/", Type::s_int(), Type::s_float(), util::mkref(BIT_FLOAT_DIV))

                ._add("+", Type::s_float(), Type::s_int(), util::mkref(BIT_FLOAT_ADD))
                ._add("-", Type::s_float(), Type::s_int(), util::mkref(BIT_FLOAT_SUB))
                ._add("*", Type::s_float(), Type::s_int(), util::mkref(BIT_FLOAT_MUL))
                ._add("/", Type::s_float(), Type::s_int(), util::mkref(BIT_FLOAT_DIV))

                ._add("=", Type::s_int(), Type::s_int(), util::mkref(BIT_INT_EQ))
                ._add("<=", Type::s_int(), Type::s_int(), util::mkref(BIT_INT_LE))
                ._add("<", Type::s_int(), Type::s_int(), util::mkref(BIT_INT_LT))
                ._add(">=", Type::s_int(), Type::s_int(), util::mkref(BIT_INT_GE))
                ._add(">", Type::s_int(), Type::s_int(), util::mkref(BIT_INT_GT))
                ._add("!=", Type::s_int(), Type::s_int(), util::mkref(BIT_INT_NE))

                ._add("=", Type::s_float(), Type::s_float(), util::mkref(BIT_FLOAT_EQ))
                ._add("<=", Type::s_float(), Type::s_float(), util::mkref(BIT_FLOAT_LE))
                ._add("<", Type::s_float(), Type::s_float(), util::mkref(BIT_FLOAT_LT))
                ._add(">=", Type::s_float(), Type::s_float(), util::mkref(BIT_FLOAT_GE))
                ._add(">", Type::s_float(), Type::s_float(), util::mkref(BIT_FLOAT_GT))
                ._add("!=", Type::s_float(), Type::s_float(), util::mkref(BIT_FLOAT_NE))

                ._add("=", Type::s_int(), Type::s_float(), util::mkref(BIT_FLOAT_EQ))
                ._add("<=", Type::s_int(), Type::s_float(), util::mkref(BIT_FLOAT_LE))
                ._add("<", Type::s_int(), Type::s_float(), util::mkref(BIT_FLOAT_LT))
                ._add(">=", Type::s_int(), Type::s_float(), util::mkref(BIT_FLOAT_GE))
                ._add(">", Type::s_int(), Type::s_float(), util::mkref(BIT_FLOAT_GT))
                ._add("!=", Type::s_int(), Type::s_float(), util::mkref(BIT_FLOAT_NE))

                ._add("=", Type::s_float(), Type::s_int(), util::mkref(BIT_FLOAT_EQ))
                ._add("<=", Type::s_float(), Type::s_int(), util::mkref(BIT_FLOAT_LE))
                ._add("<", Type::s_float(), Type::s_int(), util::mkref(BIT_FLOAT_LT))
                ._add(">=", Type::s_float(), Type::s_int(), util::mkref(BIT_FLOAT_GE))
                ._add(">", Type::s_float(), Type::s_int(), util::mkref(BIT_FLOAT_GT))
                ._add("!=", Type::s_float(), Type::s_int(), util::mkref(BIT_FLOAT_NE))

                ._add("+", Type::s_string(), Type::s_bool(), util::mkref(BIT_STRING_ADD))
                ._add("+", Type::s_string(), Type::s_int(), util::mkref(BIT_STRING_ADD))
                ._add("+", Type::s_string(), Type::s_float(), util::mkref(BIT_STRING_ADD))
                ._add("+", Type::s_string(), Type::s_string(), util::mkref(BIT_STRING_ADD))
                ._add("+", Type::s_bool(), Type::s_string(), util::mkref(BIT_STRING_ADD))
                ._add("+", Type::s_int(), Type::s_string(), util::mkref(BIT_STRING_ADD))
                ._add("+", Type::s_float(), Type::s_string(), util::mkref(BIT_STRING_ADD))
            ;
        }

        std::map<std::string
               , std::map<util::sref<Type const>
                        , std::map<util::sref<Type const>
                                 , util::sref<Operation const>>>> _map;
    };

    struct PreUnaryOpMap {
        static PreUnaryOpMap const& instance()
        {
            static PreUnaryOpMap inst;
            return inst;
        }

        util::sref<Operation const> query(std::string const& op, util::sref<Type const> rhs) const
        {
            if (Type::bad() == rhs) {
                return util::mkref(BAD_OPERATION);
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
        PreUnaryOpMap& _add(std::string const& op_id
                          , util::sref<Type const> rhs
                          , util::sref<Operation const> oper)
        {
            _map[op_id].insert(std::make_pair(rhs, oper));
            return *this;
        }
    private:
        PreUnaryOpMap()
        {
            (*this)
                ._add("+", Type::s_int(), util::mkref(BIT_POSI_INT))
                ._add("-", Type::s_int(), util::mkref(BIT_NEGA_INT))

                ._add("+", Type::s_float(), util::mkref(BIT_POSI_FLOAT))
                ._add("-", Type::s_float(), util::mkref(BIT_NEGA_FLOAT))
            ;
        }

        std::map<std::string, std::map<util::sref<Type const>, util::sref<Operation const>>> _map;
    };

}

util::sref<Operation const> Operation::queryBinary(misc::position const& pos
                                                 , std::string const& op
                                                 , util::sref<Type const> lhs
                                                 , util::sref<Type const> rhs)
{
    try {
        return BinaryOpMap::instance().query(op, lhs, rhs);
    } catch (BadOpException) {
        error::binaryOpNotAvai(pos, op, lhs->name(), rhs->name());
        return util::mkref(BAD_OPERATION);
    }
}

util::sref<Operation const> Operation::queryPreUnary(misc::position const& pos
                                                   , std::string const& op
                                                   , util::sref<Type const> rhs)
{
    try {
        return PreUnaryOpMap::instance().query(op, rhs);
    } catch (BadOpException) {
        error::preUnaryOpNotAvai(pos, op, rhs->name());
        return util::mkref(BAD_OPERATION);
    }
}
