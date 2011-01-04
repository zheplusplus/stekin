#include <algorithm>

#include "stmt-nodes.h"

using namespace grammar;

void block::add(util::sptr<stmt_base const>&& stmt)
{
    _flow.push_back(std::move(stmt));
}

void block::add(util::sptr<struct func_def const>&& func_def)
{
    _func_defs.push_back(std::move(func_def));
}

void block::compile(proto::scope const* scope) const 
{
    std::for_each(_func_defs.begin()
                , _func_defs.end()
                , [&](util::sptr<func_def const> const& def)
                  {
                      def->compile(scope);
                  });
    std::for_each(_flow.begin()
                , _flow.end()
                , [&](util::sptr<stmt_base const> const& stmt)
                  {
                      stmt->compile(scope);
                  });
}

void arithmetics::compile(proto::scope const* scope) const 
{
    scope->make_arith(pos, expr->compile(scope));
}

void branch::compile(proto::scope const* scope) const 
{
    proto::scope const* valid_scope = scope->create_branch_scope();
    valid.compile(valid_scope);
    proto::scope const* invalid_scope = scope->create_branch_scope();
    invalid.compile(invalid_scope);
    scope->branch(pos, condition->compile(scope), valid_scope->as_stmt(), invalid_scope->as_stmt());
}

void loop::compile(proto::scope const* scope) const 
{
    proto::scope const* loop_scope = scope->create_loop_scope();
    body.compile(loop_scope);
    scope->loop(pos, condition->compile(scope), loop_scope->as_stmt());
}

void func_ret::compile(proto::scope const* scope) const 
{
    scope->func_ret(pos, ret_val->compile(scope));
}

void var_def::compile(proto::scope const* scope) const 
{
    scope->def_var(pos, name, init->compile(scope));
}

void func_ret_nothing::compile(proto::scope const* scope) const
{
    scope->func_ret_nothing(pos);
}
