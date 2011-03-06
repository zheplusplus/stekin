#include <algorithm>

#include "stmt-nodes.h"
#include "../output/statement-writer.h"

using namespace inst;

branch::branch(misc::pos_type const& pos
             , util::sptr<expr_base const> p
             , util::sptr<stmt_base const> c
             , util::sptr<stmt_base const> a)
    : predicate(std::move(p))
    , consequence(std::move(c))
    , alternative(std::move(a))
{
    check_condition_type(pos, predicate->typeof());
}

void arithmetics::write() const
{
    expr->write();
    output::end_of_statement();
}

void branch::write() const
{
    output::branch_if();
    predicate->write();
    consequence->write();
    output::branch_else();
    alternative->write();
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
