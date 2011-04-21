#include <algorithm>
#include <vector>
#include <map>

#include "test-common.h"
#include "../../proto/stmt-nodes.h"
#include "../../proto/expr-nodes.h"
#include "../../proto/scope.h"
#include "../../proto/general-scope.h"
#include "../../proto/global-scope.h"
#include "../../proto/function.h"
#include "../../instance/node-base.h"
#include "../../instance/inst-mediate.h"
#include "../../util/string.h"

using namespace test;
using namespace proto;

namespace {

    util::sref<symbol_table> nul_symbols()
    {
        return util::sref<symbol_table>(NULL);
    }

    util::sptr<inst::mediate_base> nul_mediate()
    {
        return std::move(util::sptr<inst::mediate_base>(NULL));
    }

    util::sptr<inst::Expression const> nul_inst_expr()
    {
        return std::move(util::sptr<inst::Expression const>(NULL));
    }

    std::list<util::sptr<function>> func_entities;

    util::sptr<Expression const> nul_proto_expr()
    {
        return std::move(util::sptr<Expression const>(NULL));
    }

    util::sptr<scope> mkscope()
    {
        return std::move(util::mkmptr(new global_scope));
    }

    struct dummy_stmt
        : public Statement
    {
        util::sptr<inst::mediate_base> inst(util::sref<inst::scope>) const
        {
            return std::move(nul_mediate());
        }
    };

    std::vector<util::sptr<scope>> func_scope_entities;
    symbol_table phony_symbols;

}

void block::add_stmt(util::sptr<Statement const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

util::sptr<inst::mediate_base> block::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(SCOPE_BEGIN);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement const> const& stmt)
                  {
                      stmt->inst(nul_inst_scope);
                  });
    data_tree::actual_one()(SCOPE_END);
    return std::move(nul_mediate());
}

block scope::deliver()
{
    return std::move(_block);
}

util::sptr<inst::mediate_base> func_ret::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, RETURN);
    ret_val->inst(nul_inst_scope);
    return std::move(nul_mediate());
}

util::sptr<inst::mediate_base> func_ret_nothing::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, RETURN_NOTHING);
    return std::move(nul_mediate());
}

util::sptr<inst::mediate_base> var_def::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, VAR_DEF, name);
    init->inst(nul_inst_scope);
    return std::move(nul_mediate());
}

util::sptr<inst::mediate_base> branch::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, BRANCH);
    _predicate->inst(nul_inst_scope);
    _consequence.inst(nul_inst_scope);
    _alternative.inst(nul_inst_scope);
    return std::move(nul_mediate());
}

util::sptr<inst::mediate_base> arithmetics::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, ARITHMETICS);
    expr->inst(nul_inst_scope);
    return std::move(nul_mediate());
}

util::sptr<inst::Expression const> bool_literal::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, BOOLEAN, util::str(value));
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> IntLiteral::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, INTEGER, util::str(value));
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> float_literal::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, FLOATING, util::str(value));
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> reference::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, REFERENCE, name);
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> call::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, CALL, func->name, args.size(), false);
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& arg)
                  {
                      arg->inst(nul_inst_scope);
                  });
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> func_reference::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, FUNC_REFERENCE, func->name, func->param_names.size(), false);
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> binary_op::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, BINARY_OP, op);
    lhs->inst(nul_inst_scope);
    rhs->inst(nul_inst_scope);
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> pre_unary_op::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, PRE_UNARY_OP, op);
    rhs->inst(nul_inst_scope);
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> conjunction::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, BINARY_OP, "&&");
    lhs->inst(nul_inst_scope);
    rhs->inst(nul_inst_scope);
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> disjunction::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, BINARY_OP, "||");
    lhs->inst(nul_inst_scope);
    rhs->inst(nul_inst_scope);
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> negation::inst(util::sref<inst::scope>) const
{
    data_tree::actual_one()(pos, PRE_UNARY_OP, "!");
    rhs->inst(nul_inst_scope);
    return std::move(nul_inst_expr());
}

util::sptr<Expression const> scope::make_bool(misc::pos_type const& pos, bool value) const
{
    return std::move(util::mkptr(new bool_literal(pos, value)));
}

util::sptr<Expression const> scope::make_int(misc::pos_type const& pos, mpz_class const& value) const
{
    return std::move(util::mkptr(new IntLiteral(pos, value)));
}

util::sptr<Expression const> scope::make_float(misc::pos_type const& pos, mpf_class const& value) const
{
    return std::move(util::mkptr(new float_literal(pos, value)));
}

util::sptr<Expression const> scope::make_binary(misc::pos_type const& pos
                                             , util::sptr<Expression const> lhs
                                             , std::string const& op
                                             , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new binary_op(pos, std::move(lhs), op, std::move(rhs))));
}

util::sptr<Expression const> scope::make_pre_unary(misc::pos_type const& pos
                                                , std::string const& op
                                                , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new pre_unary_op(pos, op, std::move(rhs))));
}

util::sptr<Expression const> scope::make_conj(misc::pos_type const& pos
                                           , util::sptr<Expression const> lhs
                                           , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new conjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<Expression const> scope::make_disj(misc::pos_type const& pos
                                           , util::sptr<Expression const> lhs
                                           , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new disjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<Expression const> scope::make_nega(misc::pos_type const& pos, util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new negation(pos, std::move(rhs))));
}

void scope::add_stmt(util::sptr<Statement const> stmt)
{
    _block.add_stmt(std::move(stmt));
}

util::sptr<Expression const> general_scope::make_ref(misc::pos_type const& pos, std::string const& name)
{
    return std::move(util::mkptr(new reference(pos, name)));
}

util::sptr<Expression const> general_scope::make_call(misc::pos_type const& pos
                                                   , std::string const& name
                                                   , std::vector<util::sptr<Expression const>> args) const
{
    func_entities.push_back(std::move(util::mkmptr(new function(pos
                                                              , name
                                                              , std::vector<std::string>(args.size())
                                                              , util::mkref(phony_symbols)
                                                              , false))));
    return std::move(util::mkptr(new call(pos, *func_entities.back(), std::move(args))));
}

util::sptr<Expression const> general_scope::make_func_reference(misc::pos_type const& pos
                                                             , std::string const& name
                                                             , int param_count) const
{
    func_entities.push_back(std::move(util::mkmptr(new function(pos
                                                              , name
                                                              , std::vector<std::string>(param_count)
                                                              , util::mkref(phony_symbols)
                                                              , false))));
    return std::move(util::mkptr(new func_reference(pos, *func_entities.back())));
}

void general_scope::def_var(misc::pos_type const& pos, std::string const& name)
{
    data_tree::actual_one()(pos, SCOPE_VAR_DEF, name);
}

util::sptr<scope> general_scope::create_branch_scope()
{
    return std::move(mkscope());
}

util::sref<function> general_scope::declare(misc::pos_type const& pos
                                          , std::string const& name
                                          , std::vector<std::string> const& param_names
                                          , bool hint_func_ret_void)
{
    data_tree::actual_one()(pos, FUNC_DECL, name, param_names.size(), hint_func_ret_void);
    std::for_each(param_names.begin()
                , param_names.end()
                , [&](std::string const& param)
                  {
                      data_tree::actual_one()(pos, PARAMETER, param);
                  });
    func_entities.push_back(std::move(util::mkmptr(new function(pos
                                                              , name
                                                              , param_names
                                                              , util::mkref(phony_symbols)
                                                              , hint_func_ret_void))));
    return *func_entities.back();
}

global_scope::global_scope() {}

function::function(misc::pos_type const& p
                 , std::string const& n
                 , std::vector<std::string> const& params
                 , util::sref<symbol_table const>
                 , bool func_hint_void_return)
    : pos(p)
    , name(n)
    , param_names(params)
    , hint_void_return(func_hint_void_return)
{
    func_scope_entities.push_back(std::move(mkscope()));
}
