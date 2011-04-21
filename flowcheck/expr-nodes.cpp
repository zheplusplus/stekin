#include <algorithm>
#include <map>

#include "expr-nodes.h"
#include "../util/string.h"
#include "../report/errors.h"
#include "../proto/scope.h"
#include "../proto/expr-nodes.h"

using namespace flchk;

namespace {

    static util::sptr<Expression const> make_fake_expr(misc::position const& pos)
    {
        return std::move(util::mkptr(new BoolLiteral(pos, false)));
    }

    struct literal_binary_operation {
        virtual util::sptr<Expression const> operate(misc::position const& pos
                                                  , mpz_class const& lhs
                                                  , mpz_class const& rhs) const = 0;
        virtual util::sptr<Expression const> operate(misc::position const& pos
                                                  , mpz_class const& lhs
                                                  , mpf_class const& rhs) const = 0;
        virtual util::sptr<Expression const> operate(misc::position const& pos
                                                  , mpf_class const& lhs
                                                  , mpz_class const& rhs) const = 0;
        virtual util::sptr<Expression const> operate(misc::position const& pos
                                                  , mpf_class const& lhs
                                                  , mpf_class const& rhs) const = 0;
    };

    template <typename _OpFunc>
    struct binary_op_implement
        : literal_binary_operation
    {
        template <typename _LHS, typename _RHS>
        util::sptr<Expression const> operate_impl(misc::position const& pos
                                               , _LHS const& lhs
                                               , _RHS const& rhs) const
        {
            return std::move(_OpFunc()(pos, lhs, rhs));
        }

        util::sptr<Expression const> operate(misc::position const& pos
                                          , mpz_class const& lhs
                                          , mpz_class const& rhs) const
        {
            return std::move(operate_impl(pos, lhs, rhs));
        }

        util::sptr<Expression const> operate(misc::position const& pos
                                          , mpz_class const& lhs
                                          , mpf_class const& rhs) const
        {
            return std::move(operate_impl(pos, lhs, rhs));
        }

        util::sptr<Expression const> operate(misc::position const& pos
                                          , mpf_class const& lhs
                                          , mpz_class const& rhs) const
        {
            return std::move(operate_impl(pos, lhs, rhs));
        }

        util::sptr<Expression const> operate(misc::position const& pos
                                          , mpf_class const& lhs
                                          , mpf_class const& rhs) const
        {
            return std::move(operate_impl(pos, lhs, rhs));
        }
    };

    struct lt_binary {
        template <typename _LHS, typename _RHS>
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , _LHS const& lhs
                                             , _RHS const& rhs) const
        {
            return std::move(util::mkptr(new BoolLiteral(pos, lhs < rhs)));
        }
    };
    binary_op_implement<lt_binary> lt_binary_obj;

    struct le_binary {
        template <typename _LHS, typename _RHS>
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , _LHS const& lhs
                                             , _RHS const& rhs) const
        {
            return std::move(util::mkptr(new BoolLiteral(pos, lhs <= rhs)));
        }
    };
    binary_op_implement<le_binary> le_binary_obj;

    struct ge_binary {
        template <typename _LHS, typename _RHS>
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , _LHS const& lhs
                                             , _RHS const& rhs) const
        {
            return std::move(util::mkptr(new BoolLiteral(pos, lhs >= rhs)));
        }
    };
    binary_op_implement<ge_binary> ge_binary_obj;

    struct gt_binary {
        template <typename _LHS, typename _RHS>
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , _LHS const& lhs
                                             , _RHS const& rhs) const
        {
            return std::move(util::mkptr(new BoolLiteral(pos, lhs > rhs)));
        }
    };
    binary_op_implement<gt_binary> gt_binary_obj;

    struct eq_binary {
        template <typename _LHS, typename _RHS>
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , _LHS const& lhs
                                             , _RHS const& rhs) const
        {
            return std::move(util::mkptr(new BoolLiteral(pos, lhs == rhs)));
        }
    };
    binary_op_implement<eq_binary> eq_binary_obj;

    struct ne_binary {
        template <typename _LHS, typename _RHS>
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , _LHS const& lhs
                                             , _RHS const& rhs) const
        {
            return std::move(util::mkptr(new BoolLiteral(pos, lhs != rhs)));
        }
    };
    binary_op_implement<ne_binary> ne_binary_obj;

    struct add_binary {
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , mpz_class const& lhs
                                             , mpz_class const& rhs) const
        {
            return std::move(util::mkptr(new IntLiteral(pos, lhs + rhs)));
        }

        template <typename _LHS, typename _RHS>
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , _LHS const& lhs
                                             , _RHS const& rhs) const
        {
            return std::move(util::mkptr(new FloatLiteral(pos, lhs + rhs)));
        }
    };
    binary_op_implement<add_binary> add_binary_obj;

    struct sub_binary {
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , mpz_class const& lhs
                                             , mpz_class const& rhs) const
        {
            return std::move(util::mkptr(new IntLiteral(pos, lhs - rhs)));
        }

        template <typename _LHS, typename _RHS>
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , _LHS const& lhs
                                             , _RHS const& rhs) const
        {
            return std::move(util::mkptr(new FloatLiteral(pos, lhs - rhs)));
        }
    };
    binary_op_implement<sub_binary> sub_binary_obj;

    struct mul_binary {
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , mpz_class const& lhs
                                             , mpz_class const& rhs) const
        {
            return std::move(util::mkptr(new IntLiteral(pos, lhs * rhs)));
        }

        template <typename _LHS, typename _RHS>
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , _LHS const& lhs
                                             , _RHS const& rhs) const
        {
            return std::move(util::mkptr(new FloatLiteral(pos, lhs * rhs)));
        }
    };
    binary_op_implement<mul_binary> mul_binary_obj;

    struct div_binary {
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , mpz_class const& lhs
                                             , mpz_class const& rhs) const
        {
            if (0 == rhs) {
                error::binary_op_not_avai(pos, "/", "int", "integer literal(0)");
                return std::move(make_fake_expr(pos));
            }
            return std::move(util::mkptr(new IntLiteral(pos, lhs / rhs)));
        }

        util::sptr<Expression const> operator()(misc::position const& pos
                                             , mpf_class const& lhs
                                             , mpz_class const& rhs) const
        {
            if (0 == rhs) {
                error::binary_op_not_avai(pos, "/", "int", "integer literal(0)");
                return std::move(make_fake_expr(pos));
            }
            return std::move(util::mkptr(new FloatLiteral(pos, lhs / rhs)));
        }

        template <typename _LHS, typename _RHS>
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , _LHS const& lhs
                                             , _RHS const& rhs) const
        {
            return std::move(util::mkptr(new FloatLiteral(pos, lhs / rhs)));
        }
    };
    binary_op_implement<div_binary> div_binary_obj;

    struct mod_binary {
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , mpz_class const& lhs
                                             , mpz_class const& rhs) const
        {
            if (0 == rhs) {
                error::binary_op_not_avai(pos, "%", "int", "integer literal(0)");
                return std::move(make_fake_expr(pos));
            }
            return std::move(util::mkptr(new IntLiteral(pos, lhs % rhs)));
        }

        template <typename _LHS, typename _RHS>
        util::sptr<Expression const> operator()(misc::position const& pos
                                             , _LHS const&
                                             , _RHS const&) const
        {
            error::binary_op_not_avai(pos, "%", "int or float", "int or float");
            return std::move(make_fake_expr(pos));
        }
    };
    binary_op_implement<mod_binary> mod_binary_obj;

    std::map<std::string, util::sref<literal_binary_operation const>> make_cmp_op_map()
    {
        std::map<std::string, util::sref<literal_binary_operation const>> map;
        map.insert(std::make_pair("<", util::mkref(lt_binary_obj)));
        map.insert(std::make_pair("<=", util::mkref(le_binary_obj)));
        map.insert(std::make_pair(">=", util::mkref(ge_binary_obj)));
        map.insert(std::make_pair(">", util::mkref(gt_binary_obj)));
        map.insert(std::make_pair("=", util::mkref(eq_binary_obj)));
        map.insert(std::make_pair("!=", util::mkref(ne_binary_obj)));
        return map;
    }

    std::map<std::string, util::sref<literal_binary_operation const>> make_op_map()
    {
        std::map<std::string, util::sref<literal_binary_operation const>> map(make_cmp_op_map());
        map.insert(std::make_pair("+", util::mkref(add_binary_obj)));
        map.insert(std::make_pair("-", util::mkref(sub_binary_obj)));
        map.insert(std::make_pair("*", util::mkref(mul_binary_obj)));
        map.insert(std::make_pair("/", util::mkref(div_binary_obj)));
        map.insert(std::make_pair("%", util::mkref(mod_binary_obj)));
        return map;
    }

    std::map<std::string, util::sref<literal_binary_operation const>> const COMPARE_OPS(make_cmp_op_map());
    std::map<std::string, util::sref<literal_binary_operation const>> const ALL_OPS(make_op_map());

}

util::sptr<proto::Expression const> pre_unary_op::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_pre_unary(pos, op_img, rhs->compile(scope)));
}

bool pre_unary_op::is_literal() const
{
    return rhs->is_literal();
}

bool pre_unary_op::bool_value() const
{
    error::cond_not_bool(pos, type_name());
    return false;
}

std::string pre_unary_op::type_name() const
{
    return '(' + op_img + rhs->type_name() + ')';
}

util::sptr<Expression const> pre_unary_op::fold() const
{
    return std::move(rhs->fold()->as_rhs(pos, op_img));
}

util::sptr<proto::Expression const> binary_op::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_binary(pos, lhs->compile(scope), op_img, rhs->compile(scope)));
}

bool binary_op::is_literal() const
{
    return lhs->is_literal() && rhs->is_literal();
}

bool binary_op::bool_value() const
{
    if (COMPARE_OPS.end() == COMPARE_OPS.find(op_img)) {
        error::cond_not_bool(pos, type_name());
        return false;
    }
    return fold()->bool_value();
}

std::string binary_op::type_name() const
{
    return '(' + lhs->type_name() + op_img + rhs->type_name() + ')';
}

util::sptr<Expression const> binary_op::fold() const
{
    return std::move(rhs->fold()->as_rhs(pos, op_img, std::move(lhs->fold())));
}

util::sptr<proto::Expression const> conjunction::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_conj(pos, lhs->compile(scope), rhs->compile(scope)));
}

bool conjunction::is_literal() const
{
    return lhs->is_literal() && rhs->is_literal();
}

bool conjunction::bool_value() const
{
    return lhs->bool_value() && rhs->bool_value();
}

std::string conjunction::type_name() const
{
    return '(' + lhs->type_name() + "&&" + rhs->type_name() + ')';
}

util::sptr<Expression const> conjunction::fold() const
{
    if (is_literal()) {
        return std::move(util::mkptr(new BoolLiteral(pos, lhs->bool_value() && rhs->bool_value())));
    }
    return std::move(util::mkptr(new conjunction(pos, std::move(lhs->fold()), std::move(rhs->fold()))));
}

util::sptr<proto::Expression const> disjunction::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_disj(pos, lhs->compile(scope), rhs->compile(scope)));
}

bool disjunction::is_literal() const
{
    return lhs->is_literal() && rhs->is_literal();
}

bool disjunction::bool_value() const
{
    return lhs->bool_value() || rhs->bool_value();
}

std::string disjunction::type_name() const
{
    return '(' + lhs->type_name() + "||" + rhs->type_name() + ')';
}

util::sptr<Expression const> disjunction::fold() const
{
    if (is_literal()) {
        return std::move(util::mkptr(new BoolLiteral(pos, lhs->bool_value() || rhs->bool_value())));
    }
    return std::move(util::mkptr(new disjunction(pos, std::move(lhs->fold()), std::move(rhs->fold()))));
}

util::sptr<proto::Expression const> negation::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_nega(pos, rhs->compile(scope)));
}

bool negation::is_literal() const
{
    return rhs->is_literal();
}

bool negation::bool_value() const
{
    return !rhs->bool_value();
}

std::string negation::type_name() const
{
    return "(!" + rhs->type_name() + ')';
}

util::sptr<Expression const> negation::fold() const
{
    if (is_literal()) {
        return std::move(util::mkptr(new BoolLiteral(pos, !rhs->bool_value())));
    }
    return std::move(util::mkptr(new negation(pos, std::move(rhs->fold()))));
}

util::sptr<proto::Expression const> reference::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_ref(pos, name));
}

std::string reference::type_name() const
{
    return "(Reference(" + name + "))";
}

util::sptr<Expression const> reference::fold() const
{
    return std::move(util::mkptr(new reference(pos, name)));
}

util::sptr<proto::Expression const> BoolLiteral::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_bool(pos, value));
}

bool BoolLiteral::is_literal() const
{
    return true;
}

bool BoolLiteral::bool_value() const
{
    return value;
}

std::string BoolLiteral::type_name() const
{
    return "(bool(" + util::str(value) + "))";
}

util::sptr<Expression const> BoolLiteral::fold() const
{
    return std::move(util::mkptr(new BoolLiteral(pos, value)));
}

util::sptr<Expression const> BoolLiteral::operate(misc::position const& op_pos
                                                , std::string const& op_img
                                                , mpz_class const&) const
{
    error::binary_op_not_avai(op_pos, op_img, "bool", "int");
    return std::move(make_fake_expr(op_pos));
}

util::sptr<Expression const> BoolLiteral::operate(misc::position const& op_pos
                                                , std::string const& op_img
                                                , mpf_class const&) const
{
    error::binary_op_not_avai(op_pos, op_img, "bool", "float");
    return std::move(make_fake_expr(op_pos));
}

util::sptr<Expression const> BoolLiteral::operate(misc::position const& op_pos
                                                , std::string const& op_img
                                                , bool rhs) const
{
    if ("=" == op_img) {
        return std::move(util::mkptr(new BoolLiteral(op_pos, value == rhs)));
    }
    return std::move(util::mkptr(new BoolLiteral(op_pos, value != rhs)));
}

util::sptr<Expression const> BoolLiteral::as_rhs(misc::position const& op_pos
                                               , std::string const& op_img
                                               , util::sptr<Expression const> lhs) const
{
    return std::move(lhs->operate(op_pos, op_img, value));
}

util::sptr<Expression const> BoolLiteral::as_rhs(misc::position const& op_pos, std::string const& op_img) const
{
    error::pre_unary_op_not_avai(op_pos, op_img, "bool");
    return std::move(make_fake_expr(op_pos));
}

util::sptr<proto::Expression const> IntLiteral::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_int(pos, value));
}

bool IntLiteral::is_literal() const
{
    return true;
}

bool IntLiteral::bool_value() const
{
    error::cond_not_bool(pos, type_name());
    return false;
}

std::string IntLiteral::type_name() const
{
    return "(int(" + util::str(value) + "))";
}

util::sptr<Expression const> IntLiteral::fold() const
{
    return std::move(util::mkptr(new IntLiteral(pos, value)));
}

util::sptr<Expression const> IntLiteral::operate(misc::position const& op_pos
                                               , std::string const& op_img
                                               , mpz_class const& rhs) const
{
    return std::move(ALL_OPS.find(op_img)->second->operate(op_pos, value, rhs));
}

util::sptr<Expression const> IntLiteral::operate(misc::position const& op_pos
                                               , std::string const& op_img
                                               , mpf_class const& rhs) const
{
    return std::move(ALL_OPS.find(op_img)->second->operate(op_pos, value, rhs));
}

util::sptr<Expression const> IntLiteral::operate(misc::position const& op_pos
                                               , std::string const& op_img
                                               , bool) const
{
    error::binary_op_not_avai(op_pos, op_img, "int", "bool");
    return std::move(make_fake_expr(op_pos));
}

util::sptr<Expression const> IntLiteral::as_rhs(misc::position const& op_pos
                                              , std::string const& op_img
                                              , util::sptr<Expression const> lhs) const
{
    return std::move(lhs->operate(op_pos, op_img, value));
}

util::sptr<Expression const> IntLiteral::as_rhs(misc::position const& op_pos, std::string const& op_img) const
{
    if ("-" == op_img) {
        return std::move(util::mkptr(new IntLiteral(op_pos, -value)));
    }
    return std::move(util::mkptr(new IntLiteral(op_pos, value)));
}

util::sptr<proto::Expression const> FloatLiteral::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_float(pos, value));
}

bool FloatLiteral::is_literal() const
{
    return true;
}

bool FloatLiteral::bool_value() const
{
    error::cond_not_bool(pos, type_name());
    return false;
}

std::string FloatLiteral::type_name() const
{
    return "(float(" + util::str(value) + "))";
}

util::sptr<Expression const> FloatLiteral::fold() const
{
    return std::move(util::mkptr(new FloatLiteral(pos, value)));
}

util::sptr<Expression const> FloatLiteral::operate(misc::position const& op_pos
                                                 , std::string const& op_img
                                                 , mpz_class const& rhs) const
{
    return std::move(ALL_OPS.find(op_img)->second->operate(op_pos, value, rhs));
}

util::sptr<Expression const> FloatLiteral::operate(misc::position const& op_pos
                                                 , std::string const& op_img
                                                 , mpf_class const& rhs) const
{
    return std::move(ALL_OPS.find(op_img)->second->operate(op_pos, value, rhs));
}

util::sptr<Expression const> FloatLiteral::operate(misc::position const& op_pos
                                                 , std::string const& op_img
                                                 , bool) const
{
    error::binary_op_not_avai(op_pos, op_img, "float", "bool");
    return std::move(make_fake_expr(op_pos));
}

util::sptr<Expression const> FloatLiteral::as_rhs(misc::position const& op_pos
                                                , std::string const& op_img
                                                , util::sptr<Expression const> lhs) const
{
    return std::move(lhs->operate(op_pos, op_img, value));
}

util::sptr<Expression const> FloatLiteral::as_rhs(misc::position const& op_pos, std::string const& op_img) const
{
    if ("-" == op_img) {
        return std::move(util::mkptr(new FloatLiteral(op_pos, -value)));
    }
    return std::move(util::mkptr(new FloatLiteral(op_pos, value)));
}

util::sptr<proto::Expression const> call::compile(util::sref<proto::scope> scope) const
{
    std::vector<util::sptr<proto::Expression const>> arguments;
    arguments.reserve(args.size());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& expr)
                  {
                      arguments.push_back(expr->compile(scope));
                  });
    return std::move(scope->make_call(pos, name, std::move(arguments)));
}

std::string call::type_name() const
{
    if (args.empty()) {
        return "(call(" + name + "))";
    }
    std::string args_names;
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& arg)
                  {
                      args_names += (arg->type_name() + ", ");
                  });
    return "(call(" + name + ")(" + args_names.substr(0, args_names.length() - 2) + "))";
}

util::sptr<Expression const> call::fold() const
{
    std::vector<util::sptr<Expression const>> args_fold;
    args_fold.reserve(args.size());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& expr)
                  {
                      args_fold.push_back(std::move(expr->fold()));
                  });
    return std::move(util::mkptr(new call(pos, name, std::move(args_fold))));
}

util::sptr<proto::Expression const> func_reference::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_func_reference(pos, name, param_count));
}

std::string func_reference::type_name() const
{
    return "(func reference(" + name + ")@" + util::str(param_count) + ')';
}

util::sptr<Expression const> func_reference::fold() const
{
    return std::move(util::mkptr(new func_reference(pos, name, param_count)));
}
