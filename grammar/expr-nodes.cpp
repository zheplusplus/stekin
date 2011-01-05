#include <algorithm>

#include "expr-nodes.h"

using namespace grammar;

proto::expr_base const* pre_unary_oper::compile(proto::scope const* scope) const
{
    return scope->make_pre_unary(pos, op_img, rhs->compile(scope));
}

proto::expr_base const* binary_oper::compile(proto::scope const* scope) const
{
    return scope->make_binary(pos, lhs->compile(scope), op_img, rhs->compile(scope));
}

proto::expr_base const* conjunction::compile(proto::scope const* scope) const
{
    return scope->make_conj(pos, lhs->compile(scope), rhs->compile(scope));
}

proto::expr_base const* disjunction::compile(proto::scope const* scope) const
{
    return scope->make_disj(pos, lhs->compile(scope), rhs->compile(scope));
}

proto::expr_base const* negation::compile(proto::scope const* scope) const
{
    return scope->make_nega(pos, rhs->compile(scope));
}

proto::expr_base const* reference::compile(proto::scope const* scope) const
{
    return scope->make_ref(pos, name);
}

proto::expr_base const* bool_literal::compile(proto::scope const* scope) const
{
    return scope->make_bool(pos, value);
}

proto::expr_base const* int_literal::compile(proto::scope const* scope) const
{
    return scope->make_int(pos, value);
}

proto::expr_base const* float_literal::compile(proto::scope const* scope) const
{
    return scope->make_float(pos, value);
}

proto::expr_base const* call::compile(proto::scope const* scope) const
{
    std::vector<proto::expr_base const*> arguments;
    arguments.reserve(args.size());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<expr_base const> const& expr)
                  {
                      arguments.push_back(expr->compile(scope));
                  });
    return scope->make_call(pos, name, arguments);
}
