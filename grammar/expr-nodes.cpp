#include <algorithm>

#include "expr-nodes.h"

using namespace grammar;

util::sptr<proto::expr_base const> pre_unary_op::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_pre_unary(pos, op_img, rhs->compile(scope)));
}

util::sptr<proto::expr_base const> binary_op::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_binary(pos, lhs->compile(scope), op_img, rhs->compile(scope)));
}

util::sptr<proto::expr_base const> conjunction::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_conj(pos, lhs->compile(scope), rhs->compile(scope)));
}

util::sptr<proto::expr_base const> disjunction::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_disj(pos, lhs->compile(scope), rhs->compile(scope)));
}

util::sptr<proto::expr_base const> negation::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_nega(pos, rhs->compile(scope)));
}

util::sptr<proto::expr_base const> reference::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_ref(pos, name));
}

util::sptr<proto::expr_base const> bool_literal::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_bool(pos, value));
}

util::sptr<proto::expr_base const> int_literal::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_int(pos, value));
}

util::sptr<proto::expr_base const> float_literal::compile(util::sref<proto::scope> scope) const
{
    return std::move(scope->make_float(pos, value));
}

util::sptr<proto::expr_base const> call::compile(util::sref<proto::scope> scope) const
{
    std::vector<util::sptr<proto::expr_base const>> arguments;
    arguments.reserve(args.size());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<expr_base const> const& expr)
                  {
                      arguments.push_back(expr->compile(scope));
                  });
    return std::move(scope->make_call(pos, name, std::move(arguments)));
}
