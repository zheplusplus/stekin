#include <algorithm>

#include "stmt-nodes.h"
#include "function.h"
#include "filter.h"
#include "symbol-table.h"
#include "../proto/function.h"
#include "../proto/stmt-nodes.h"

using namespace flchk;

util::sptr<proto::Statement const> Arithmetics::compile(util::sref<proto::Scope> scope) const 
{
    return std::move(util::mkptr(new proto::Arithmetics(pos, expr->compile(scope))));
}

util::sptr<proto::Statement const> Branch::compile(util::sref<proto::Scope> scope) const 
{
    util::sptr<proto::Scope> consq_scope(new proto::Scope);
    util::sptr<proto::Scope> alter_scope(new proto::Scope);
    consequence.compile(*consq_scope);
    alternative.compile(*alter_scope);
    return std::move(util::mkptr(new proto::Branch(pos
                                                 , predicate->compile(scope)
                                                 , consq_scope->deliver()
                                                 , alter_scope->deliver())));
}

util::sptr<proto::Statement const> VarDef::compile(util::sref<proto::Scope> scope) const 
{
    util::sptr<proto::Expression const> init_value(init->compile(scope));
    _symbols->defVar(pos, name);
    return std::move(util::mkptr(new proto::VarDef(pos, name, std::move(init_value))));
}

util::sptr<proto::Statement const> Return::compile(util::sref<proto::Scope> scope) const 
{
    return std::move(util::mkptr(new proto::Return(pos, ret_val->compile(scope))));
}

util::sptr<proto::Statement const> ReturnNothing::compile(util::sref<proto::Scope>) const
{
    return std::move(util::mkptr(new proto::ReturnNothing(pos)));
}
