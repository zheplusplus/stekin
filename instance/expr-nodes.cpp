#include <algorithm>

#include "expr-nodes.h"
#include "function.h"

using namespace inst;

type const* int_literal::typeof() const
{
    return type::BIT_INT;
}

type const* float_literal::typeof() const
{
    return type::BIT_FLOAT;
}

type const* bool_literal::typeof() const
{
    return type::BIT_BOOL;
}

type const* reference::typeof() const
{
    return var.vtype;
}

type const* call::typeof() const
{
    return func->get_return_type();
}

type const* binary_op::typeof() const
{
    return op->ret_type;
}

type const* pre_unary_op::typeof() const
{
    return op->ret_type;
}

type const* conjunction::typeof() const
{
    return type::BIT_BOOL;
}

type const* disjunction::typeof() const
{
    return type::BIT_BOOL;
}

type const* negation::typeof() const
{
    return type::BIT_BOOL;
}
