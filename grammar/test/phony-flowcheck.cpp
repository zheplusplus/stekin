#include <algorithm>

#include "test-common.h"
#include "../../flowcheck/filter.h"
#include "../../flowcheck/accumulator.h"
#include "../../flowcheck/block.h"
#include "../../flowcheck/expr-nodes.h"
#include "../../flowcheck/stmt-nodes.h"
#include "../../flowcheck/function.h"
#include "../../proto/node-base.h"
#include "../../util/string.h"

using namespace test;
using namespace flchk;

namespace {

    util::sptr<proto::stmt_base const> nul_proto_stmt()
    {
        return std::move(util::sptr<proto::stmt_base const>(NULL));
    }

    util::sptr<proto::expr_base const> nul_proto_expr()
    {
        return std::move(util::sptr<proto::expr_base const>(NULL));
    }

    util::sptr<flchk::expr_base const> nul_flchk_expr()
    {
        return std::move(util::sptr<flchk::expr_base const>(NULL));
    }

    struct branch_consequence
        : public stmt_base
    {
        branch_consequence(misc::pos_type const& pos, util::sptr<expr_base const> p, block c)
            : stmt_base(pos)
            , predicate(std::move(p))
            , consequence(std::move(c))
        {}

        util::sptr<proto::stmt_base const> compile(util::sref<proto::scope>) const
        {
            data_tree::actual_one()(pos, BRANCH_CONSQ_ONLY);
            predicate->compile(nulscope);
            data_tree::actual_one()(CONSEQUENCE);
            consequence.compile(nulscope);
            return std::move(nul_proto_stmt());
        }

        util::sptr<expr_base const> const predicate;
        block const consequence;
    };

    struct branch_alternative
        : public stmt_base
    {
        branch_alternative(misc::pos_type const& pos, util::sptr<expr_base const> p, block a)
            : stmt_base(pos)
            , predicate(std::move(p))
            , alternative(std::move(a))
        {}

        util::sptr<proto::stmt_base const> compile(util::sref<proto::scope>) const
        {
            data_tree::actual_one()(pos, BRANCH_ALTER_ONLY);
            predicate->compile(nulscope);
            data_tree::actual_one()(ALTERNATIVE);
            alternative.compile(nulscope);
            return std::move(nul_proto_stmt());
        }

        util::sptr<expr_base const> const predicate;
        block const alternative;
    };

}

void function::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, FUNC_DEF, name);
    std::for_each(param_names.begin()
                , param_names.end()
                , [&](std::string const& param)
                  {
                      data_tree::actual_one()(pos, PARAMETER, param);
                  });
    body.compile(nulscope);
}

void block::add_stmt(util::sptr<stmt_base const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

void block::def_func(misc::pos_type const& pos
                   , std::string const& name
                   , std::vector<std::string> const& param_names
                   , block body
                   , bool)
{
    _funcs.push_back(std::move(util::mkptr(new function(pos
                                                      , name
                                                      , param_names
                                                      , std::move(body)
                                                      , false))));
}

void block::compile(util::sref<proto::scope>) const 
{
    data_tree::actual_one()(BLOCK_BEGIN);
    std::for_each(_funcs.begin()
                , _funcs.end()
                , [&](util::sptr<function const> const& func)
                  {
                      func->compile(nulscope);
                  });

    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<stmt_base const> const& stmt)
                  {
                      stmt->compile(nulscope);
                  });
    data_tree::actual_one()(BLOCK_END);
}

void accumulator::add_func_ret(misc::pos_type const& pos, util::sptr<expr_base const> ret_val)
{
    _block.add_stmt(std::move(util::mkptr(new func_ret(pos, std::move(ret_val)))));
}

void accumulator::add_func_ret_nothing(misc::pos_type const& pos)
{
    _block.add_stmt(std::move(util::mkptr(new func_ret_nothing(pos))));
}

void accumulator::add_arith(misc::pos_type const& pos, util::sptr<expr_base const> expr)
{
    _block.add_stmt(std::move(util::mkptr(new arithmetics(pos, std::move(expr)))));
}

void accumulator::add_branch(misc::pos_type const& pos
                           , util::sptr<expr_base const> predicate
                           , accumulator consequence
                           , accumulator alternative)
{
    _block.add_stmt(std::move(util::mkptr(new branch(pos
                                                   , std::move(predicate)
                                                   , std::move(consequence.deliver())
                                                   , std::move(alternative.deliver())))));
}

void accumulator::add_branch(misc::pos_type const& pos
                           , util::sptr<expr_base const> predicate
                           , accumulator consequence)
{
    _block.add_stmt(std::move(util::mkptr(new branch_consequence(pos
                                                               , std::move(predicate)
                                                               , std::move(consequence._block)))));
}

void accumulator::add_branch_alt_only(misc::pos_type const& pos
                                    , util::sptr<expr_base const> predicate
                                    , accumulator alternative)
{
    _block.add_stmt(std::move(util::mkptr(new branch_alternative(pos
                                                               , std::move(predicate)
                                                               , std::move(alternative._block)))));
}

void accumulator::def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const> init)
{
    _block.add_stmt(std::move(util::mkptr(new var_def(pos, name, std::move(init)))));
}

void accumulator::def_func(misc::pos_type const& pos
                         , std::string const& name
                         , std::vector<std::string> const& param_names
                         , accumulator body)
{
    _block.def_func(pos, name, param_names, std::move(body._block), false);
}

block accumulator::deliver()
{
    return std::move(_block);
}

void filter::add_func_ret(misc::pos_type const& pos, util::sptr<expr_base const> ret_val)
{
    _accumulator.add_func_ret(pos, std::move(ret_val));
}

void filter::add_func_ret_nothing(misc::pos_type const& pos)
{
    _accumulator.add_func_ret_nothing(pos);
}

void filter::add_arith(misc::pos_type const& pos, util::sptr<expr_base const> expr)
{
    _accumulator.add_arith(pos, std::move(expr));
}

void filter::add_branch(misc::pos_type const& pos
                      , util::sptr<expr_base const> predicate
                      , util::sptr<filter> consequence
                      , util::sptr<filter> alternative)
{
    _accumulator.add_branch(pos
                          , std::move(predicate)
                          , std::move(consequence->_accumulator)
                          , std::move(alternative->_accumulator));
}

void filter::add_branch(misc::pos_type const& pos
                      , util::sptr<expr_base const> predicate
                      , util::sptr<filter> consequence)
{
    _accumulator.add_branch(pos, std::move(predicate), std::move(consequence->_accumulator));
}

void filter::add_branch_alt_only(misc::pos_type const& pos
                               , util::sptr<expr_base const> predicate
                               , util::sptr<filter> alternative)
{
    _accumulator.add_branch_alt_only(pos, std::move(predicate), std::move(alternative->_accumulator));
}

void filter::def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const> init)
{
    _accumulator.def_var(pos, name, std::move(init));
}

void filter::def_func(misc::pos_type const& pos
                    , std::string const& name
                    , std::vector<std::string> const& param_names
                    , util::sptr<filter> body)
{
    _accumulator.def_func(pos, name, param_names, std::move(body->_accumulator));
}

block filter::deliver()
{
    return std::move(_accumulator.deliver());
}

void symbol_def_filter::def_var(misc::pos_type const& pos
                              , std::string const& name
                              , util::sptr<expr_base const> init)
{
    filter::def_var(pos, name + ' ' + VAR_DEF_FILTERED.type_img, std::move(init));
}

void symbol_def_filter::def_func(misc::pos_type const& pos
                               , std::string const& name
                               , std::vector<std::string> const& param_names
                               , util::sptr<filter> body)
{
    filter::def_func(pos, name + ' ' + FUNC_DEF_FILTERED.type_img, param_names, std::move(body));
}

util::sptr<proto::stmt_base const> arithmetics::compile(util::sref<proto::scope>) const 
{
    data_tree::actual_one()(pos, ARITHMETICS);
    expr->compile(nulscope);
    return std::move(nul_proto_stmt());
}

util::sptr<proto::stmt_base const> branch::compile(util::sref<proto::scope>) const 
{
    data_tree::actual_one()(pos, BRANCH);
    predicate->compile(nulscope);
    data_tree::actual_one()(CONSEQUENCE);
    consequence.compile(nulscope);
    data_tree::actual_one()(ALTERNATIVE);
    alternative.compile(nulscope);
    return std::move(nul_proto_stmt());
}

util::sptr<proto::stmt_base const> var_def::compile(util::sref<proto::scope>) const 
{
    data_tree::actual_one()(pos, VAR_DEF, name);
    init->compile(nulscope);
    return std::move(nul_proto_stmt());
}

util::sptr<proto::stmt_base const> func_ret::compile(util::sref<proto::scope>) const 
{
    data_tree::actual_one()(pos, RETURN);
    ret_val->compile(nulscope);
    return std::move(nul_proto_stmt());
}

util::sptr<proto::stmt_base const> func_ret_nothing::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, RETURN_NOTHING);
    return std::move(nul_proto_stmt());
}

util::sptr<proto::expr_base const> pre_unary_op::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, PRE_UNARY_OP, op_img);
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> binary_op::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BINARY_OP, op_img);
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> conjunction::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BINARY_OP, "&&");
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> disjunction::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BINARY_OP, "||");
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> negation::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, PRE_UNARY_OP, "!");
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> reference::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, REFERENCE, name);
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> bool_literal::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BOOLEAN, util::str(value));
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> int_literal::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, INTEGER, util::str(value));
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> float_literal::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, FLOATING, util::str(value));
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> call::compile(util::sref<proto::scope> s) const
{
    data_tree::actual_one()(pos, CALL, name, args.size());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<expr_base const> const& arg)
                  {
                      arg->compile(s);
                  });
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> func_reference::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, FUNC_REFERENCE, name, param_count);
    return std::move(nul_proto_expr());
}

bool expr_base::is_literal() const
{
    return false;
}

bool expr_base::bool_value() const
{
    return false;
}

util::sptr<expr_base const> expr_base::operate(misc::pos_type const&
                                             , std::string const&
                                             , mpz_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> expr_base::operate(misc::pos_type const&
                                             , std::string const&
                                             , mpf_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> expr_base::operate(misc::pos_type const&, std::string const&, bool) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> expr_base::as_rhs(misc::pos_type const&
                                            , std::string const&
                                            , util::sptr<expr_base const>) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> expr_base::as_rhs(misc::pos_type const&, std::string const&) const
{
    return std::move(nul_flchk_expr());
}

bool pre_unary_op::is_literal() const
{
    return false;
}

bool pre_unary_op::bool_value() const
{
    return false;
}

std::string pre_unary_op::type_name() const
{
    return "";
}

util::sptr<expr_base const> pre_unary_op::fold() const
{
    return std::move(nul_flchk_expr());
}

bool binary_op::is_literal() const
{
    return false;
}

bool binary_op::bool_value() const
{
    return false;
}

std::string binary_op::type_name() const
{
    return "";
}

util::sptr<expr_base const> binary_op::fold() const
{
    return std::move(nul_flchk_expr());
}

bool conjunction::is_literal() const
{
    return false;
}

bool conjunction::bool_value() const
{
    return false;
}

std::string conjunction::type_name() const
{
    return "";
}

util::sptr<expr_base const> conjunction::fold() const
{
    return std::move(nul_flchk_expr());
}

bool disjunction::is_literal() const
{
    return false;
}

bool disjunction::bool_value() const
{
    return false;
}

std::string disjunction::type_name() const
{
    return "";
}

util::sptr<expr_base const> disjunction::fold() const
{
    return std::move(nul_flchk_expr());
}

bool negation::is_literal() const
{
    return false;
}

bool negation::bool_value() const
{
    return false;
}

std::string negation::type_name() const
{
    return "";
}

util::sptr<expr_base const> negation::fold() const
{
    return std::move(nul_flchk_expr());
}

std::string reference::type_name() const
{
    return "";
}

util::sptr<expr_base const> reference::fold() const
{
    return std::move(nul_flchk_expr());
}

bool bool_literal::is_literal() const
{
    return false;
}

bool bool_literal::bool_value() const
{
    return false;
}

std::string bool_literal::type_name() const
{
    return "";
}

util::sptr<expr_base const> bool_literal::fold() const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> bool_literal::operate(misc::pos_type const&
                                                , std::string const&
                                                , mpz_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> bool_literal::operate(misc::pos_type const&
                                                , std::string const&
                                                , mpf_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> bool_literal::operate(misc::pos_type const&, std::string const&, bool) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> bool_literal::as_rhs(misc::pos_type const&
                                               , std::string const&
                                               , util::sptr<expr_base const>) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> bool_literal::as_rhs(misc::pos_type const&, std::string const&) const
{
    return std::move(nul_flchk_expr());
}

bool int_literal::is_literal() const
{
    return false;
}

bool int_literal::bool_value() const
{
    return false;
}

std::string int_literal::type_name() const
{
    return "";
}

util::sptr<expr_base const> int_literal::fold() const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> int_literal::operate(misc::pos_type const&
                                               , std::string const&
                                               , mpz_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> int_literal::operate(misc::pos_type const&
                                               , std::string const&
                                               , mpf_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> int_literal::operate(misc::pos_type const&, std::string const&, bool) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> int_literal::as_rhs(misc::pos_type const&
                                              , std::string const&
                                              , util::sptr<expr_base const>) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> int_literal::as_rhs(misc::pos_type const&, std::string const&) const
{
    return std::move(nul_flchk_expr());
}

bool float_literal::is_literal() const
{
    return false;
}

bool float_literal::bool_value() const
{
    return false;
}

std::string float_literal::type_name() const
{
    return "";
}

util::sptr<expr_base const> float_literal::fold() const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> float_literal::operate(misc::pos_type const&
                                                 , std::string const&
                                                 , mpz_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> float_literal::operate(misc::pos_type const&
                                                 , std::string const&
                                                 , mpf_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> float_literal::operate(misc::pos_type const&, std::string const&, bool) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> float_literal::as_rhs(misc::pos_type const&
                                                , std::string const&
                                                , util::sptr<expr_base const>) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> float_literal::as_rhs(misc::pos_type const&, std::string const&) const
{
    return std::move(nul_flchk_expr());
}

std::string call::type_name() const
{
    return "";
}

util::sptr<expr_base const> call::fold() const
{
    return std::move(nul_flchk_expr());
}

std::string func_reference::type_name() const
{
    return "";
}

util::sptr<expr_base const> func_reference::fold() const
{
    return std::move(nul_flchk_expr());
}
