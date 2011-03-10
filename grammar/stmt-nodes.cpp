#include "stmt-nodes.h"
#include "function.h"
#include "../proto/node-base.h"
#include "../proto/function.h"

using namespace grammar;

void arithmetics::compile(util::sref<proto::scope> scope) const 
{
    scope->add_arith(pos, expr->compile(scope));
}

void branch::compile(util::sref<proto::scope> scope) const 
{
    util::sptr<proto::scope> consequence_scope = scope->create_branch_scope();
    consequence.compile(*consequence_scope);
    util::sptr<proto::scope> alternative_scope = scope->create_branch_scope();
    alternative.compile(*alternative_scope);
    scope->add_branch(pos
                    , predicate->compile(scope)
                    , std::move(consequence_scope)
                    , std::move(alternative_scope));
}

void func_ret::compile(util::sref<proto::scope> scope) const 
{
    scope->add_func_ret(pos, ret_val->compile(scope));
}

void var_def::compile(util::sref<proto::scope> scope) const 
{
    scope->def_var(pos, name, init->compile(scope));
}

void func_ret_nothing::compile(util::sref<proto::scope> scope) const
{
    scope->add_func_ret_nothing(pos);
}
