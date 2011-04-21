#include <algorithm>

#include "stmt-nodes.h"
#include "function.h"
#include "../proto/scope.h"
#include "../proto/stmt-nodes.h"

using namespace flchk;

util::sptr<proto::Statement const> arithmetics::compile(util::sref<proto::scope> scope) const 
{
    return std::move(util::mkptr(new proto::arithmetics(pos, expr->compile(scope))));
}

util::sptr<proto::Statement const> branch::compile(util::sref<proto::scope> scope) const 
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

util::sptr<proto::Statement const> var_def::compile(util::sref<proto::scope> scope) const 
{
    scope->def_var(pos, name);
    return std::move(util::mkptr(new proto::var_def(pos, name, init->compile(scope))));
}

util::sptr<proto::Statement const> func_ret::compile(util::sref<proto::scope> scope) const 
{
    return std::move(util::mkptr(new proto::func_ret(pos, ret_val->compile(scope))));
}

util::sptr<proto::Statement const> func_ret_nothing::compile(util::sref<proto::scope>) const
{
    return std::move(util::mkptr(new proto::func_ret_nothing(pos)));
}
