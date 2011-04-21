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

    util::sptr<proto::Statement const> nul_proto_stmt()
    {
        return std::move(util::sptr<proto::Statement const>(NULL));
    }

    util::sptr<proto::Expression const> nul_proto_expr()
    {
        return std::move(util::sptr<proto::Expression const>(NULL));
    }

    util::sptr<flchk::Expression const> nul_flchk_expr()
    {
        return std::move(util::sptr<flchk::Expression const>(NULL));
    }

    struct branch_consequence
        : public Statement
    {
        branch_consequence(misc::position const& pos, util::sptr<Expression const> p, Block c)
            : Statement(pos)
            , predicate(std::move(p))
            , consequence(std::move(c))
        {}

        util::sptr<proto::Statement const> compile(util::sref<proto::scope>) const
        {
            data_tree::actual_one()(pos, BRANCH_CONSQ_ONLY);
            predicate->compile(nulscope);
            data_tree::actual_one()(CONSEQUENCE);
            consequence.compile(nulscope);
            return std::move(nul_proto_stmt());
        }

        util::sptr<Expression const> const predicate;
        Block const consequence;
    };

    struct branch_alternative
        : public Statement
    {
        branch_alternative(misc::position const& pos, util::sptr<Expression const> p, Block a)
            : Statement(pos)
            , predicate(std::move(p))
            , alternative(std::move(a))
        {}

        util::sptr<proto::Statement const> compile(util::sref<proto::scope>) const
        {
            data_tree::actual_one()(pos, BRANCH_ALTER_ONLY);
            predicate->compile(nulscope);
            data_tree::actual_one()(ALTERNATIVE);
            alternative.compile(nulscope);
            return std::move(nul_proto_stmt());
        }

        util::sptr<Expression const> const predicate;
        Block const alternative;
    };

}

void Function::compile(util::sref<proto::scope>) const
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

void Block::add_stmt(util::sptr<Statement const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

void Block::def_func(misc::position const& pos
                   , std::string const& name
                   , std::vector<std::string> const& param_names
                   , Block body
                   , bool)
{
    _funcs.push_back(std::move(util::mkptr(new Function(pos
                                                      , name
                                                      , param_names
                                                      , std::move(body)
                                                      , false))));
}

void Block::compile(util::sref<proto::scope>) const 
{
    data_tree::actual_one()(BLOCK_BEGIN);
    std::for_each(_funcs.begin()
                , _funcs.end()
                , [&](util::sptr<Function const> const& func)
                  {
                      func->compile(nulscope);
                  });

    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement const> const& stmt)
                  {
                      stmt->compile(nulscope);
                  });
    data_tree::actual_one()(BLOCK_END);
}

void accumulator::add_func_ret(misc::position const& pos, util::sptr<Expression const> ret_val)
{
    _Block.add_stmt(std::move(util::mkptr(new func_ret(pos, std::move(ret_val)))));
}

void accumulator::add_func_ret_nothing(misc::position const& pos)
{
    _Block.add_stmt(std::move(util::mkptr(new func_ret_nothing(pos))));
}

void accumulator::add_arith(misc::position const& pos, util::sptr<Expression const> expr)
{
    _Block.add_stmt(std::move(util::mkptr(new arithmetics(pos, std::move(expr)))));
}

void accumulator::add_branch(misc::position const& pos
                           , util::sptr<Expression const> predicate
                           , accumulator consequence
                           , accumulator alternative)
{
    _Block.add_stmt(std::move(util::mkptr(new branch(pos
                                                   , std::move(predicate)
                                                   , std::move(consequence.deliver())
                                                   , std::move(alternative.deliver())))));
}

void accumulator::add_branch(misc::position const& pos
                           , util::sptr<Expression const> predicate
                           , accumulator consequence)
{
    _Block.add_stmt(std::move(util::mkptr(new branch_consequence(pos
                                                               , std::move(predicate)
                                                               , std::move(consequence._Block)))));
}

void accumulator::add_branch_alt_only(misc::position const& pos
                                    , util::sptr<Expression const> predicate
                                    , accumulator alternative)
{
    _Block.add_stmt(std::move(util::mkptr(new branch_alternative(pos
                                                               , std::move(predicate)
                                                               , std::move(alternative._Block)))));
}

void accumulator::def_var(misc::position const& pos, std::string const& name, util::sptr<Expression const> init)
{
    _Block.add_stmt(std::move(util::mkptr(new var_def(pos, name, std::move(init)))));
}

void accumulator::def_func(misc::position const& pos
                         , std::string const& name
                         , std::vector<std::string> const& param_names
                         , accumulator body)
{
    _Block.def_func(pos, name, param_names, std::move(body._Block), false);
}

Block accumulator::deliver()
{
    return std::move(_Block);
}

void filter::add_func_ret(misc::position const& pos, util::sptr<Expression const> ret_val)
{
    _accumulator.add_func_ret(pos, std::move(ret_val));
}

void filter::add_func_ret_nothing(misc::position const& pos)
{
    _accumulator.add_func_ret_nothing(pos);
}

void filter::add_arith(misc::position const& pos, util::sptr<Expression const> expr)
{
    _accumulator.add_arith(pos, std::move(expr));
}

void filter::add_branch(misc::position const& pos
                      , util::sptr<Expression const> predicate
                      , util::sptr<filter> consequence
                      , util::sptr<filter> alternative)
{
    _accumulator.add_branch(pos
                          , std::move(predicate)
                          , std::move(consequence->_accumulator)
                          , std::move(alternative->_accumulator));
}

void filter::add_branch(misc::position const& pos
                      , util::sptr<Expression const> predicate
                      , util::sptr<filter> consequence)
{
    _accumulator.add_branch(pos, std::move(predicate), std::move(consequence->_accumulator));
}

void filter::add_branch_alt_only(misc::position const& pos
                               , util::sptr<Expression const> predicate
                               , util::sptr<filter> alternative)
{
    _accumulator.add_branch_alt_only(pos, std::move(predicate), std::move(alternative->_accumulator));
}

void filter::def_var(misc::position const& pos, std::string const& name, util::sptr<Expression const> init)
{
    _accumulator.def_var(pos, name, std::move(init));
}

void filter::def_func(misc::position const& pos
                    , std::string const& name
                    , std::vector<std::string> const& param_names
                    , util::sptr<filter> body)
{
    _accumulator.def_func(pos, name, param_names, std::move(body->_accumulator));
}

Block filter::deliver()
{
    return std::move(_accumulator.deliver());
}

void symbol_def_filter::def_var(misc::position const& pos
                              , std::string const& name
                              , util::sptr<Expression const> init)
{
    filter::def_var(pos, name + VAR_DEF_FILTERED, std::move(init));
}

void symbol_def_filter::def_func(misc::position const& pos
                               , std::string const& name
                               , std::vector<std::string> const& param_names
                               , util::sptr<filter> body)
{
    filter::def_func(pos, name + FUNC_DEF_FILTERED, param_names, std::move(body));
}

util::sptr<proto::Statement const> arithmetics::compile(util::sref<proto::scope>) const 
{
    data_tree::actual_one()(pos, ARITHMETICS);
    expr->compile(nulscope);
    return std::move(nul_proto_stmt());
}

util::sptr<proto::Statement const> branch::compile(util::sref<proto::scope>) const 
{
    data_tree::actual_one()(pos, BRANCH);
    predicate->compile(nulscope);
    data_tree::actual_one()(CONSEQUENCE);
    consequence.compile(nulscope);
    data_tree::actual_one()(ALTERNATIVE);
    alternative.compile(nulscope);
    return std::move(nul_proto_stmt());
}

util::sptr<proto::Statement const> var_def::compile(util::sref<proto::scope>) const 
{
    data_tree::actual_one()(pos, VAR_DEF, name);
    init->compile(nulscope);
    return std::move(nul_proto_stmt());
}

util::sptr<proto::Statement const> func_ret::compile(util::sref<proto::scope>) const 
{
    data_tree::actual_one()(pos, RETURN);
    ret_val->compile(nulscope);
    return std::move(nul_proto_stmt());
}

util::sptr<proto::Statement const> func_ret_nothing::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, RETURN_NOTHING);
    return std::move(nul_proto_stmt());
}

util::sptr<proto::Expression const> PreUnaryOp::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, PRE_UNARY_OP, op_img);
    return std::move(nul_proto_expr());
}

util::sptr<proto::Expression const> BinaryOp::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BINARY_OP, op_img);
    return std::move(nul_proto_expr());
}

util::sptr<proto::Expression const> Conjunction::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BINARY_OP, "&&");
    return std::move(nul_proto_expr());
}

util::sptr<proto::Expression const> Disjunction::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BINARY_OP, "||");
    return std::move(nul_proto_expr());
}

util::sptr<proto::Expression const> Negation::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, PRE_UNARY_OP, "!");
    return std::move(nul_proto_expr());
}

util::sptr<proto::Expression const> reference::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, REFERENCE, name);
    return std::move(nul_proto_expr());
}

util::sptr<proto::Expression const> BoolLiteral::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BOOLEAN, util::str(value));
    return std::move(nul_proto_expr());
}

util::sptr<proto::Expression const> IntLiteral::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, INTEGER, util::str(value));
    return std::move(nul_proto_expr());
}

util::sptr<proto::Expression const> FloatLiteral::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, FLOATING, util::str(value));
    return std::move(nul_proto_expr());
}

util::sptr<proto::Expression const> call::compile(util::sref<proto::scope> s) const
{
    data_tree::actual_one()(pos, CALL, name, args.size());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& arg)
                  {
                      arg->compile(s);
                  });
    return std::move(nul_proto_expr());
}

util::sptr<proto::Expression const> FuncReference::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, FUNC_REFERENCE, name, param_count);
    return std::move(nul_proto_expr());
}

bool Expression::is_literal() const
{
    return false;
}

bool Expression::bool_value() const
{
    return false;
}

util::sptr<Expression const> Expression::operate(misc::position const&
                                             , std::string const&
                                             , mpz_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> Expression::operate(misc::position const&
                                             , std::string const&
                                             , mpf_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> Expression::operate(misc::position const&, std::string const&, bool) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> Expression::as_rhs(misc::position const&
                                            , std::string const&
                                            , util::sptr<Expression const>) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> Expression::as_rhs(misc::position const&, std::string const&) const
{
    return std::move(nul_flchk_expr());
}

bool PreUnaryOp::is_literal() const
{
    return false;
}

bool PreUnaryOp::bool_value() const
{
    return false;
}

std::string PreUnaryOp::type_name() const
{
    return "";
}

util::sptr<Expression const> PreUnaryOp::fold() const
{
    return std::move(nul_flchk_expr());
}

bool BinaryOp::is_literal() const
{
    return false;
}

bool BinaryOp::bool_value() const
{
    return false;
}

std::string BinaryOp::type_name() const
{
    return "";
}

util::sptr<Expression const> BinaryOp::fold() const
{
    return std::move(nul_flchk_expr());
}

bool Conjunction::is_literal() const
{
    return false;
}

bool Conjunction::bool_value() const
{
    return false;
}

std::string Conjunction::type_name() const
{
    return "";
}

util::sptr<Expression const> Conjunction::fold() const
{
    return std::move(nul_flchk_expr());
}

bool Disjunction::is_literal() const
{
    return false;
}

bool Disjunction::bool_value() const
{
    return false;
}

std::string Disjunction::type_name() const
{
    return "";
}

util::sptr<Expression const> Disjunction::fold() const
{
    return std::move(nul_flchk_expr());
}

bool Negation::is_literal() const
{
    return false;
}

bool Negation::bool_value() const
{
    return false;
}

std::string Negation::type_name() const
{
    return "";
}

util::sptr<Expression const> Negation::fold() const
{
    return std::move(nul_flchk_expr());
}

std::string reference::type_name() const
{
    return "";
}

util::sptr<Expression const> reference::fold() const
{
    return std::move(nul_flchk_expr());
}

bool BoolLiteral::is_literal() const
{
    return false;
}

bool BoolLiteral::bool_value() const
{
    return false;
}

std::string BoolLiteral::type_name() const
{
    return "";
}

util::sptr<Expression const> BoolLiteral::fold() const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> BoolLiteral::operate(misc::position const&
                                                , std::string const&
                                                , mpz_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> BoolLiteral::operate(misc::position const&
                                                , std::string const&
                                                , mpf_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> BoolLiteral::operate(misc::position const&, std::string const&, bool) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> BoolLiteral::as_rhs(misc::position const&
                                               , std::string const&
                                               , util::sptr<Expression const>) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> BoolLiteral::as_rhs(misc::position const&, std::string const&) const
{
    return std::move(nul_flchk_expr());
}

bool IntLiteral::is_literal() const
{
    return false;
}

bool IntLiteral::bool_value() const
{
    return false;
}

std::string IntLiteral::type_name() const
{
    return "";
}

util::sptr<Expression const> IntLiteral::fold() const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> IntLiteral::operate(misc::position const&
                                               , std::string const&
                                               , mpz_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> IntLiteral::operate(misc::position const&
                                               , std::string const&
                                               , mpf_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> IntLiteral::operate(misc::position const&, std::string const&, bool) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> IntLiteral::as_rhs(misc::position const&
                                              , std::string const&
                                              , util::sptr<Expression const>) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> IntLiteral::as_rhs(misc::position const&, std::string const&) const
{
    return std::move(nul_flchk_expr());
}

bool FloatLiteral::is_literal() const
{
    return false;
}

bool FloatLiteral::bool_value() const
{
    return false;
}

std::string FloatLiteral::type_name() const
{
    return "";
}

util::sptr<Expression const> FloatLiteral::fold() const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> FloatLiteral::operate(misc::position const&
                                                 , std::string const&
                                                 , mpz_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> FloatLiteral::operate(misc::position const&
                                                 , std::string const&
                                                 , mpf_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> FloatLiteral::operate(misc::position const&, std::string const&, bool) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> FloatLiteral::as_rhs(misc::position const&
                                                , std::string const&
                                                , util::sptr<Expression const>) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<Expression const> FloatLiteral::as_rhs(misc::position const&, std::string const&) const
{
    return std::move(nul_flchk_expr());
}

std::string call::type_name() const
{
    return "";
}

util::sptr<Expression const> call::fold() const
{
    return std::move(nul_flchk_expr());
}

std::string FuncReference::type_name() const
{
    return "";
}

util::sptr<Expression const> FuncReference::fold() const
{
    return std::move(nul_flchk_expr());
}
