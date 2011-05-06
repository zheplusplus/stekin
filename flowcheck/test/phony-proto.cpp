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

    util::sptr<inst::Statement const> nulInstStmt()
    {
        return util::sptr<inst::Statement const>(NULL);
    }

    util::sptr<inst::Expression const> nulInstExpr()
    {
        return util::sptr<inst::Expression const>(NULL);
    }

    std::list<util::sptr<Function>> func_entities;
    std::vector<util::sptr<Scope>> func_scope_entities;

}

void Block::addTo(util::sref<inst::Scope>) {}
void Block::mediateInst(util::sref<inst::Scope>) {}

void Block::addMediate(util::sptr<inst::MediateBase> mediate)
{
    _mediates.push_back(std::move(mediate));
}

util::sptr<inst::Statement const> Block::inst(util::sref<inst::Scope>)
{
    DataTree::actualOne()(SCOPE_BEGIN);
    std::for_each(_mediates.begin()
                , _mediates.end()
                , [&](util::sptr<inst::MediateBase> const& mediate)
                  {
                      mediate->inst(nul_inst_scope);
                  });
    DataTree::actualOne()(SCOPE_END);
    return nulInstStmt();
}

void DirectInst::addTo(util::sref<inst::Scope>) {}
void DirectInst::mediateInst(util::sref<inst::Scope>) {}

util::sptr<inst::Statement const> DirectInst::inst(util::sref<inst::Scope>)
{
    return _inst(nul_inst_scope);
}

util::sptr<inst::MediateBase> Scope::inst()
{
    return std::move(_block);
}

util::sptr<inst::Statement const> Return::_inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, RETURN);
    ret_val->inst(nul_inst_scope);
    return nulInstStmt();
}

util::sptr<inst::Statement const> ReturnNothing::_inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, RETURN_NOTHING);
    return nulInstStmt();
}

util::sptr<inst::Statement const> VarDef::_inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, VAR_DEF, name);
    init->inst(nul_inst_scope);
    return nulInstStmt();
}

void BranchMediate::addTo(util::sref<inst::Scope>) {}
void BranchMediate::mediateInst(util::sref<inst::Scope>) {}

util::sptr<inst::Statement const> BranchMediate::inst(util::sref<inst::Scope>)
{
    DataTree::actualOne()(pos, BRANCH);
    predicate->inst(nul_inst_scope);
    _consequence_mediate->inst(nul_inst_scope);
    _alternative_mediate->inst(nul_inst_scope);
    return nulInstStmt();
}

util::sptr<inst::Statement const> Arithmetics::_inst(util::sref<inst::Scope>) const
{
    DataTree::actualOne()(pos, ARITHMETICS);
    expr->inst(nul_inst_scope);
    return nulInstStmt();
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

void Scope::addMediate(util::sptr<inst::MediateBase> mediate)
{
    _block->addMediate(std::move(mediate));
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
    func_entities.push_back(util::mkptr(new Function(pos, name, param_names, hint_return_void)));
    return *func_entities.back();
}

void Function::setFreeVariables(std::vector<std::string> const&) {}
