#include <algorithm>

#include "stmt-nodes.h"
#include "function.h"
#include "filter.h"
#include "symbol-table.h"
#include "../proto/function.h"
#include "../proto/stmt-nodes.h"
#include "../instance/node-base.h"

using namespace flchk;

util::sptr<proto::Statement> Arithmetics::compile(util::sref<proto::Scope> scope
                                                , util::sref<SymbolTable> st) const
{
    return util::mkptr(new proto::Arithmetics(pos, expr->compile(scope, st)));
}

util::sptr<proto::Statement> Branch::compile(util::sref<proto::Scope> scope
                                           , util::sref<SymbolTable> st) const
{
    util::sptr<proto::Scope> consq_scope(new proto::Scope);
    util::sptr<proto::Scope> alter_scope(new proto::Scope);
    consequence.compile(*consq_scope, st);
    alternative.compile(*alter_scope, st);
    return util::mkptr(new proto::BranchMediate(pos
                                              , predicate->compile(scope, st)
                                              , consq_scope->inst()
                                              , alter_scope->inst()));
}

util::sptr<proto::Statement> VarDef::compile(util::sref<proto::Scope> scope
                                           , util::sref<SymbolTable> st) const
{
    util::sptr<proto::Expression const> init_value(init->compile(scope, st));
    st->defVar(pos, name);
    return util::mkptr(new proto::VarDef(pos, name, std::move(init_value)));
}

util::sptr<proto::Statement> Return::compile(util::sref<proto::Scope> scope
                                           , util::sref<SymbolTable> st) const
{
    return util::mkptr(new proto::Return(pos, ret_val->compile(scope, st)));
}

util::sptr<proto::Statement> ReturnNothing::compile(util::sref<proto::Scope>
                                                  , util::sref<SymbolTable>) const
{
    return util::mkptr(new proto::ReturnNothing(pos));
}
