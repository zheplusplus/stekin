#include <algorithm>
#include <vector>
#include <map>

#include "test-common.h"
#include "../../proto/stmt-nodes.h"
#include "../../proto/expr-nodes.h"
#include "../../proto/scope.h"
#include "../../proto/function.h"
#include "../../proto/built-in.h"
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
        return std::move(util::mkmptr(new Scope));
    }

    std::vector<util::sptr<Scope>> func_scope_entities;

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

util::sptr<inst::Expression const> Call::inst(util::sref<inst::Scope>) const
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

util::sptr<inst::Expression const> Functor::inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, FUNCTOR, name, args.size(), false);
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& arg)
                  {
                      arg->inst(nul_inst_scope);
                  });
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

util::sptr<inst::Expression const> WriteExpr::inst(util::sref<inst::Scope>) const
{
    return std::move(nulInstExpr());
}

void Scope::addStmt(util::sptr<Statement const> stmt)
{
    _block.addStmt(std::move(stmt));
}

util::sref<Function> Scope::declare(misc::position const& pos
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
    func_entities.push_back(util::mkmptr(new Function(pos, name, param_names, hint_return_void)));
    return *func_entities.back();
}

void Function::setFreeVariables(std::vector<std::string> const&) {}
