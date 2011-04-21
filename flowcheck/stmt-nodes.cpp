#include <algorithm>

#include "stmt-nodes.h"
#include "function.h"
#include "../proto/scope.h"
#include "../proto/stmt-nodes.h"

using namespace flchk;

util::sptr<proto::Statement const> Arithmetics::compile(util::sref<proto::scope> scope) const 
{
    return std::move(util::mkptr(new proto::Arithmetics(pos, expr->compile(scope))));
}

util::sptr<proto::Statement const> Branch::compile(util::sref<proto::scope> scope) const 
{
    util::sptr<proto::scope> consq_scope(std::move(scope->create_branch_scope()));
    util::sptr<proto::scope> alter_scope(std::move(scope->create_branch_scope()));
    consequence.compile(*consq_scope);
    alternative.compile(*alter_scope);
    return std::move(util::mkptr(new proto::branch(pos
                                                 , predicate->compile(scope)
                                                 , consq_scope->deliver()
                                                 , alter_scope->deliver())));
}

util::sptr<proto::Statement const> VarDef::compile(util::sref<proto::scope> scope) const 
{
    scope->defVar(pos, name);
    return std::move(util::mkptr(new proto::VarDef(pos, name, init->compile(scope))));
}

util::sptr<proto::Statement const> Return::compile(util::sref<proto::scope> scope) const 
{
    return std::move(util::mkptr(new proto::Return(pos, ret_val->compile(scope))));
}

util::sptr<proto::Statement const> ReturnNothing::compile(util::sref<proto::scope>) const
{
    return std::move(util::mkptr(new proto::ReturnNothing(pos)));
}
