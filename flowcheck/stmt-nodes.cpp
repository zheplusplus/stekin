#include <algorithm>

#include "stmt-nodes.h"
#include "function.h"
#include "filter.h"
#include "symbol-table.h"
#include "../proto/function.h"
#include "../proto/stmt-nodes.h"

using namespace flchk;

util::sptr<proto::Statement const> Arithmetics::compile(util::sref<proto::Scope> scope
                                                      , util::sref<SymbolTable> st) const
{
    return util::mkptr(new proto::Arithmetics(pos, expr->compile(scope, st)));
}

util::sptr<proto::Statement const> Branch::compile(util::sref<proto::Scope> scope
                                                 , util::sref<SymbolTable> st) const
{
    util::sptr<proto::Scope> consq_scope(new proto::Scope);
    util::sptr<proto::Scope> alter_scope(new proto::Scope);
    consequence.compile(*consq_scope, st);
    alternative.compile(*alter_scope, st);
    return util::mkptr(new proto::Branch(pos
                                       , predicate->compile(scope, st)
                                       , consq_scope->deliver()
                                       , alter_scope->deliver()));
}

util::sptr<proto::Statement const> VarDef::compile(util::sref<proto::Scope> scope
                                                 , util::sref<SymbolTable> st) const
{
    util::sptr<proto::Expression const> init_value(init->compile(scope, st));
    st->defVar(pos, name);
    return util::mkptr(new proto::VarDef(pos, name, std::move(init_value)));
}

util::sptr<proto::Statement const> Return::compile(util::sref<proto::Scope> scope
                                                 , util::sref<SymbolTable> st) const
{
    return util::mkptr(new proto::Return(pos, ret_val->compile(scope, st)));
}

util::sptr<proto::Statement const> ReturnNothing::compile(util::sref<proto::Scope>
                                                        , util::sref<SymbolTable>) const
{
    return util::mkptr(new proto::ReturnNothing(pos));
}
