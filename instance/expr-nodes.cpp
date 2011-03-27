#include <algorithm>

#include "expr-nodes.h"
#include "function.h"
#include "operation.h"
#include "../output/func-writer.h"
#include "../output/statement-writer.h"

using namespace inst;

conjunction::conjunction(misc::pos_type const& p, util::sptr<expr_base const> l, util::sptr<expr_base const> r)
    : lhs(std::move(l))
    , rhs(std::move(r))
{
    check_condition_type(p, lhs->typeof());
    check_condition_type(p, rhs->typeof());
}

disjunction::disjunction(misc::pos_type const& p, util::sptr<expr_base const> l, util::sptr<expr_base const> r)
    : lhs(std::move(l))
    , rhs(std::move(r))
{
    check_condition_type(p, lhs->typeof());
    check_condition_type(p, rhs->typeof());
}

negation::negation(misc::pos_type const& p, util::sptr<expr_base const> r)
    : rhs(std::move(r))
{
    check_condition_type(p, rhs->typeof());
}

util::sref<type const> int_literal::typeof() const
{
    return type::BIT_INT;
}

util::sref<type const> float_literal::typeof() const
{
    return type::BIT_FLOAT;
}

util::sref<type const> bool_literal::typeof() const
{
    return type::BIT_BOOL;
}

util::sref<type const> reference::typeof() const
{
    return var.vtype;
}

util::sref<type const> call::typeof() const
{
    return func->get_return_type();
}

util::sref<type const> func_reference::typeof() const
{
    return util::mkref(_type);
}

util::sref<type const> binary_op::typeof() const
{
    return op->ret_type;
}

util::sref<type const> pre_unary_op::typeof() const
{
    return op->ret_type;
}

util::sref<type const> conjunction::typeof() const
{
    return type::BIT_BOOL;
}

util::sref<type const> disjunction::typeof() const
{
    return type::BIT_BOOL;
}

util::sref<type const> negation::typeof() const
{
    return type::BIT_BOOL;
}

void int_literal::write() const
{
    output::write_int(value);
}

void float_literal::write() const
{
    output::write_float(value);
}

void bool_literal::write() const
{
    output::write_bool(value);
}

void reference::write() const
{
    output::ref_level(var.stack_offset, var.level, typeof()->name());
}

void call::write() const
{
    output::write_call_begin(func.id());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<expr_base const> const& expr)
                  {
                       output::write_arg_seperator();
                       expr->write();
                  });
    output::write_call_end();
}

void func_reference::write() const {}

void binary_op::write() const
{
    output::begin_expr();
    lhs->write();
    output::write_operator(op->op_img);
    rhs->write();
    output::end_expr();
}

void pre_unary_op::write() const
{
    output::begin_expr();
    output::write_operator(op->op_img);
    rhs->write();
    output::end_expr();
}

void conjunction::write() const
{
    output::begin_expr();
    lhs->write();
    output::write_operator("&&");
    rhs->write();
    output::end_expr();
}

void disjunction::write() const
{
    output::begin_expr();
    lhs->write();
    output::write_operator("||");
    rhs->write();
    output::end_expr();
}

void negation::write() const
{
    output::begin_expr();
    output::write_operator("!");
    rhs->write();
    output::end_expr();
}
