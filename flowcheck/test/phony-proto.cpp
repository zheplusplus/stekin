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

    util::sptr<inst::MediateBase> nulMediate()
    {
        return std::move(util::sptr<inst::MediateBase>(NULL));
    }

    util::sptr<inst::Expression const> nulInstExpr()
    {
        return std::move(util::sptr<inst::Expression const>(NULL));
    }

    std::list<util::sptr<Function>> func_entities;

    util::sptr<Scope> mkscope()
    {
        return std::move(util::mkmptr(new GlobalScope));
    }

    std::vector<util::sptr<Scope>> func_scope_entities;
    SymbolTable phony_symbols;

}

void Block::addStmt(util::sptr<Statement const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

util::sptr<inst::MediateBase> Block::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(SCOPE_BEGIN);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement const> const& stmt)
                  {
                      stmt->inst(nul_inst_scope);
                  });
    DataTree::actualOne()(SCOPE_END);
    return std::move(nulMediate());
}

Block Scope::deliver()
{
    return std::move(_block);
}

util::sptr<inst::MediateBase> Return::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, RETURN);
    ret_val->inst(nul_inst_scope);
    return std::move(nulMediate());
}

util::sptr<inst::MediateBase> ReturnNothing::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, RETURN_NOTHING);
    return std::move(nulMediate());
}

util::sptr<inst::MediateBase> VarDef::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, VAR_DEF, name);
    init->inst(nul_inst_scope);
    return std::move(nulMediate());
}

util::sptr<inst::MediateBase> Branch::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, BRANCH);
    _predicate->inst(nul_inst_scope);
    _consequence.inst(nul_inst_scope);
    _alternative.inst(nul_inst_scope);
    return std::move(nulMediate());
}

util::sptr<inst::MediateBase> Arithmetics::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, ARITHMETICS);
    expr->inst(nul_inst_scope);
    return std::move(nulMediate());
}

util::sptr<inst::Expression const> BoolLiteral::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, BOOLEAN, util::str(value));
    return std::move(nulInstExpr());
}

util::sptr<inst::Expression const> IntLiteral::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, INTEGER, util::str(value));
    return std::move(nulInstExpr());
}

util::sptr<inst::Expression const> FloatLiteral::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, FLOATING, util::str(value));
    return std::move(nulInstExpr());
}

util::sptr<inst::Expression const> Reference::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, REFERENCE, name);
    return std::move(nulInstExpr());
}

util::sptr<inst::Expression const> call::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, CALL, func->name, args.size(), false);
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& arg)
                  {
                      arg->inst(nul_inst_scope);
                  });
    return std::move(nulInstExpr());
}

util::sptr<inst::Expression const> FuncReference::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, FUNC_REFERENCE, func->name, func->param_names.size(), false);
    return std::move(nulInstExpr());
}

util::sptr<inst::Expression const> BinaryOp::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, BINARY_OP, op);
    lhs->inst(nul_inst_scope);
    rhs->inst(nul_inst_scope);
    return std::move(nulInstExpr());
}

util::sptr<inst::Expression const> PreUnaryOp::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, PRE_UNARY_OP, op);
    rhs->inst(nul_inst_scope);
    return std::move(nulInstExpr());
}

util::sptr<inst::Expression const> Conjunction::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, BINARY_OP, "&&");
    lhs->inst(nul_inst_scope);
    rhs->inst(nul_inst_scope);
    return std::move(nulInstExpr());
}

util::sptr<inst::Expression const> Disjunction::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, BINARY_OP, "||");
    lhs->inst(nul_inst_scope);
    rhs->inst(nul_inst_scope);
    return std::move(nulInstExpr());
}

util::sptr<inst::Expression const> Negation::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, PRE_UNARY_OP, "!");
    rhs->inst(nul_inst_scope);
    return std::move(nulInstExpr());
}

util::sptr<Expression const> Scope::makeBool(misc::position const& pos, bool value) const
{
    return std::move(util::mkptr(new BoolLiteral(pos, value)));
}

util::sptr<Expression const> Scope::makeInt(misc::position const& pos, mpz_class const& value) const
{
    return std::move(util::mkptr(new IntLiteral(pos, value)));
}

util::sptr<Expression const> Scope::makeFloat(misc::position const& pos
                                            , mpf_class const& value) const
{
    return std::move(util::mkptr(new FloatLiteral(pos, value)));
}

util::sptr<Expression const> Scope::makeBinary(misc::position const& pos
                                             , util::sptr<Expression const> lhs
                                             , std::string const& op
                                             , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new BinaryOp(pos, std::move(lhs), op, std::move(rhs))));
}

util::sptr<Expression const> Scope::makePreUnary(misc::position const& pos
                                               , std::string const& op
                                               , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new PreUnaryOp(pos, op, std::move(rhs))));
}

util::sptr<Expression const> Scope::makeConj(misc::position const& pos
                                           , util::sptr<Expression const> lhs
                                           , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new Conjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<Expression const> Scope::makeDisj(misc::position const& pos
                                           , util::sptr<Expression const> lhs
                                           , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new Disjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<Expression const> Scope::makeNega(misc::position const& pos
                                           , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new Negation(pos, std::move(rhs))));
}

void Scope::addStmt(util::sptr<Statement const> stmt)
{
    _block.addStmt(std::move(stmt));
}

util::sptr<Expression const> GeneralScope::makeRef(misc::position const& pos
                                                 , std::string const& name)
{
    return std::move(util::mkptr(new Reference(pos, name)));
}

util::sptr<Expression const> GeneralScope::makeCall(misc::position const& pos
                                                  , std::string const& name
                                                  , std::vector<util::sptr<Expression const>> args)
                                                  const
{
    func_entities.push_back(std::move(
                util::mkmptr(new Function(pos
                                        , name
                                        , std::vector<std::string>(args.size())
                                        , util::mkref(phony_symbols)
                                        , false))));
    return std::move(util::mkptr(new call(pos, *func_entities.back(), std::move(args))));
}

util::sptr<Expression const> GeneralScope::makeFuncReference(misc::position const& pos
                                                           , std::string const& name
                                                           , int param_count) const
{
    func_entities.push_back(std::move(
                util::mkmptr(new Function(pos
                                        , name
                                        , std::vector<std::string>(param_count)
                                        , util::mkref(phony_symbols)
                                        , false))));
    return std::move(util::mkptr(new FuncReference(pos, *func_entities.back())));
}

void GeneralScope::defVar(misc::position const& pos, std::string const& name)
{
    DataTree::actualOne()(pos, SCOPE_VAR_DEF, name);
}

util::sptr<Scope> GeneralScope::createBranchScope()
{
    return std::move(mkscope());
}

util::sref<Function> GeneralScope::declare(misc::position const& pos
                                         , std::string const& name
                                         , std::vector<std::string> const& param_names
                                         , bool hint_return_void)
{
    DataTree::actualOne()(pos, FUNC_DECL, name, param_names.size(), hint_return_void);
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
                                                              , hint_return_void))));
    return *func_entities.back();
}

GlobalScope::GlobalScope() {}

Function::Function(misc::position const& p
                 , std::string const& n
                 , std::vector<std::string> const& params
                 , util::sref<SymbolTable const>
                 , bool func_hint_void_return)
    : pos(p)
    , name(n)
    , param_names(params)
    , hint_void_return(func_hint_void_return)
{
    func_scope_entities.push_back(std::move(mkscope()));
}
