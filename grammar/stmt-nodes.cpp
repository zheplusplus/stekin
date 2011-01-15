#include <algorithm>

#include "stmt-nodes.h"

using namespace grammar;

void block::add(util::sptr<stmt_base const> stmt)
{
    _flow.push_back(std::move(stmt));
}

void block::add(util::sptr<struct func_def const> func_def)
{
    _func_defs.push_back(std::move(func_def));
}

void block::compile(util::sref<proto::scope> scope) const 
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

void arithmetics::compile(util::sref<proto::scope> scope) const 
{
    scope->add_arith(pos, expr->compile(scope));
}

void branch::compile(util::sref<proto::scope> scope) const 
{
    util::sptr<proto::scope> valid_scope = scope->create_branch_scope();
    valid.compile(*valid_scope);
    util::sptr<proto::scope> invalid_scope = scope->create_branch_scope();
    invalid.compile(*invalid_scope);
    scope->add_branch(pos
                    , condition->compile(scope)
                    , valid_scope->extract_stmts()
                    , invalid_scope->extract_stmts());
}

void loop::compile(util::sref<proto::scope> scope) const 
{
    util::sptr<proto::scope> loop_scope = scope->create_loop_scope();
    body.compile(*loop_scope);
    scope->add_loop(pos, condition->compile(scope), loop_scope->extract_stmts());
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