#include <algorithm>

#include "stmt-nodes.h"
#include "../output/statement-writer.h"

using namespace inst;

branch::branch(misc::pos_type const& p
             , util::sptr<expr_base const> c
             , util::sptr<stmt_base const> v
             , util::sptr<stmt_base const> i)
    : condition(std::move(c))
    , valid(std::move(v))
    , invalid(std::move(i))
{
    check_condition_type(p, condition->typeof());
}

void arithmetics::write() const
{
    expr->write();
    output::end_of_statement();
}

void branch::write() const
{
    output::branch_if();
    condition->write();
    valid->write();
    output::branch_else();
    invalid->write();
}

void initialization::write() const
{
    output::ref_this_level(offset, init->typeof()->name);
    output::assign_sign();
    init->write();
    output::end_of_statement();
}

void func_ret::write() const
{
    output::return_kw();
    ret_val->write();
    output::end_of_statement();
}

void func_ret_nothing::write() const
{
    output::return_nothing();
}
