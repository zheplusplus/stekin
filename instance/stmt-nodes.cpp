#include <algorithm>

#include "stmt-instances.h"
#include "../output/statement-writer.h"

using namespace inst;

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

void loop::write() const
{
    output::loop_while();
    condition->write();
    body->write();
}

void initialization::write() const
{
    output::ref_this_level(offset, init->type->name);
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
    output::return_kw();
    output::end_of_statement();
}
