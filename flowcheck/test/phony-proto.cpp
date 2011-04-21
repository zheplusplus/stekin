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

    std::list<util::sptr<Function>> func_entities;

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

void Block::add_stmt(util::sptr<Statement const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

util::sptr<inst::mediate_base> Block::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(SCOPE_BEGIN);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement const> const& stmt)
                  {
                      stmt->inst(nul_inst_scope);
                  });
    DataTree::actualOne()(SCOPE_END);
    return std::move(nul_mediate());
}

Block scope::deliver()
{
    return std::move(_block);
}

util::sptr<inst::mediate_base> func_ret::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, RETURN);
    ret_val->inst(nul_inst_scope);
    return std::move(nul_mediate());
}

util::sptr<inst::mediate_base> func_ret_nothing::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, RETURN_NOTHING);
    return std::move(nul_mediate());
}

util::sptr<inst::mediate_base> var_def::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, VAR_DEF, name);
    init->inst(nul_inst_scope);
    return std::move(nul_mediate());
}

util::sptr<inst::mediate_base> branch::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, BRANCH);
    _predicate->inst(nul_inst_scope);
    _consequence.inst(nul_inst_scope);
    _alternative.inst(nul_inst_scope);
    return std::move(nul_mediate());
}

util::sptr<inst::mediate_base> arithmetics::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, ARITHMETICS);
    expr->inst(nul_inst_scope);
    return std::move(nul_mediate());
}

util::sptr<inst::Expression const> BoolLiteral::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, BOOLEAN, util::str(value));
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> IntLiteral::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, INTEGER, util::str(value));
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> FloatLiteral::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, FLOATING, util::str(value));
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> reference::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, REFERENCE, name);
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> call::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, CALL, func->name, args.size(), false);
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& arg)
                  {
                      arg->inst(nul_inst_scope);
                  });
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> FuncReference::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, FUNC_REFERENCE, func->name, func->param_names.size(), false);
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> BinaryOp::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, BINARY_OP, op);
    lhs->inst(nul_inst_scope);
    rhs->inst(nul_inst_scope);
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> PreUnaryOp::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, PRE_UNARY_OP, op);
    rhs->inst(nul_inst_scope);
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> Conjunction::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, BINARY_OP, "&&");
    lhs->inst(nul_inst_scope);
    rhs->inst(nul_inst_scope);
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> Disjunction::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, BINARY_OP, "||");
    lhs->inst(nul_inst_scope);
    rhs->inst(nul_inst_scope);
    return std::move(nul_inst_expr());
}

util::sptr<inst::Expression const> Negation::inst(util::sref<inst::scope>) const
{
    DataTree::actualOne()(pos, PRE_UNARY_OP, "!");
    rhs->inst(nul_inst_scope);
    return std::move(nul_inst_expr());
}

util::sptr<Expression const> scope::make_bool(misc::position const& pos, bool value) const
{
    return std::move(util::mkptr(new BoolLiteral(pos, value)));
}

util::sptr<Expression const> scope::make_int(misc::position const& pos, mpz_class const& value) const
{
    return std::move(util::mkptr(new IntLiteral(pos, value)));
}

util::sptr<Expression const> scope::make_float(misc::position const& pos, mpf_class const& value) const
{
    return std::move(util::mkptr(new FloatLiteral(pos, value)));
}

util::sptr<Expression const> scope::make_binary(misc::position const& pos
                                             , util::sptr<Expression const> lhs
                                             , std::string const& op
                                             , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new BinaryOp(pos, std::move(lhs), op, std::move(rhs))));
}

util::sptr<Expression const> scope::make_pre_unary(misc::position const& pos
                                                , std::string const& op
                                                , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new PreUnaryOp(pos, op, std::move(rhs))));
}

util::sptr<Expression const> scope::make_conj(misc::position const& pos
                                           , util::sptr<Expression const> lhs
                                           , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new Conjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<Expression const> scope::make_disj(misc::position const& pos
                                           , util::sptr<Expression const> lhs
                                           , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new Disjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<Expression const> scope::make_nega(misc::position const& pos, util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new Negation(pos, std::move(rhs))));
}

void scope::add_stmt(util::sptr<Statement const> stmt)
{
    _block.add_stmt(std::move(stmt));
}

util::sptr<Expression const> general_scope::make_ref(misc::position const& pos, std::string const& name)
{
    return std::move(util::mkptr(new reference(pos, name)));
}

util::sptr<Expression const> general_scope::make_call(misc::position const& pos
                                                   , std::string const& name
                                                   , std::vector<util::sptr<Expression const>> args) const
{
    func_entities.push_back(std::move(util::mkmptr(new Function(pos
                                                              , name
                                                              , std::vector<std::string>(args.size())
                                                              , util::mkref(phony_symbols)
                                                              , false))));
    return std::move(util::mkptr(new call(pos, *func_entities.back(), std::move(args))));
}

util::sptr<Expression const> general_scope::make_FuncReference(misc::position const& pos
                                                             , std::string const& name
                                                             , int param_count) const
{
    func_entities.push_back(std::move(util::mkmptr(new Function(pos
                                                              , name
                                                              , std::vector<std::string>(param_count)
                                                              , util::mkref(phony_symbols)
                                                              , false))));
    return std::move(util::mkptr(new FuncReference(pos, *func_entities.back())));
}

void general_scope::def_var(misc::position const& pos, std::string const& name)
{
    DataTree::actualOne()(pos, SCOPE_VAR_DEF, name);
}

util::sptr<scope> general_scope::create_branch_scope()
{
    return std::move(mkscope());
}

util::sref<Function> general_scope::declare(misc::position const& pos
                                          , std::string const& name
                                          , std::vector<std::string> const& param_names
                                          , bool hint_func_ret_void)
{
    DataTree::actualOne()(pos, FUNC_DECL, name, param_names.size(), hint_func_ret_void);
    std::for_each(param_names.begin()
                , param_names.end()
                , [&](std::string const& param)
                  {
                      DataTree::actualOne()(pos, PARAMETER, param);
                  });
    func_entities.push_back(std::move(util::mkmptr(new Function(pos
                                                              , name
                                                              , param_names
                                                              , util::mkref(phony_symbols)
                                                              , hint_func_ret_void))));
    return *func_entities.back();
}

global_scope::global_scope() {}

Function::Function(misc::position const& p
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
