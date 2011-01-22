#include <algorithm>

#include "expr-nodes.h"
#include "func-templ.h"
#include "../instance/expr-nodes.h"
#include "../instance/type.h"

using namespace proto;

util::sptr<inst::expr_base const> bool_literal::inst(util::sref<inst::scope const>) const
{
    return std::move(util::mkptr(new inst::bool_literal(value)));
}

util::sptr<inst::expr_base const> int_literal::inst(util::sref<inst::scope const>) const
{
    return std::move(util::mkptr(new inst::int_literal(value.get_si())));
}

util::sptr<inst::expr_base const> float_literal::inst(util::sref<inst::scope const>) const
{
    return std::move(util::mkptr(new inst::float_literal(value.get_d())));
}

util::sptr<inst::expr_base const> reference::inst(util::sref<inst::scope const> scope) const
{
    return std::move(util::mkptr(new inst::reference(scope->query_var(pos, name))));
}

util::sptr<inst::expr_base const> call::inst(util::sref<inst::scope const> scope) const
{
    std::vector<util::sptr<inst::expr_base const>> arg_instances;
    std::vector<inst::type const*> arg_types;
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<expr_base const> const& expr)
                  {
                      util::sptr<inst::expr_base const> arg(std::move(expr->inst(scope)));
                      arg_instances.push_back(std::move(arg));
                      arg_types.push_back(arg->typeof());
                  });
    return std::move(util::mkptr(new inst::call(func->inst(pos, scope, arg_types), std::move(arg_instances))));
}

util::sptr<inst::expr_base const> binary_op::inst(util::sref<inst::scope const> scope) const
{
    util::sptr<inst::expr_base const> left = lhs->inst(scope);
    util::sptr<inst::expr_base const> right = rhs->inst(scope);
    inst::type const* ltype = left->typeof();
    inst::type const* rtype = right->typeof();
    return std::move(util::mkptr(new inst::binary_op(std::move(left)
                                                   , scope->query_binary(pos, op, ltype, rtype)
                                                   , std::move(right))));
}

util::sptr<inst::expr_base const> pre_unary_op::inst(util::sref<inst::scope const> scope) const
{
    util::sptr<inst::expr_base const> right = rhs->inst(scope);
    inst::type const* rtype = right->typeof();
    return std::move(util::mkptr(
                new inst::pre_unary_op(scope->query_pre_unary(pos, op, rtype), std::move(right))));
}

util::sptr<inst::expr_base const> conjunction::inst(util::sref<inst::scope const> scope) const
{
    return std::move(util::mkptr(new inst::conjunction(std::move(lhs->inst(scope))
                                                     , std::move(rhs->inst(scope)))));
}

util::sptr<inst::expr_base const> disjunction::inst(util::sref<inst::scope const> scope) const
{
    return std::move(util::mkptr(new inst::disjunction(std::move(lhs->inst(scope))
                                                     , std::move(rhs->inst(scope)))));
}

util::sptr<inst::expr_base const> negation::inst(util::sref<inst::scope const> scope) const
{
    return std::move(util::mkptr(new inst::negation(std::move(rhs->inst(scope)))));
}
