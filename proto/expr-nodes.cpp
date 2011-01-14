#include <algorithm>

#include "expr-nodes.h"
#include "func-templ.h"
#include "../instance/expr-nodes.h"
#include "../instance/type.h"

using namespace proto;

inst::int_literal const* int_literal::inst(inst::scope const* scope) const
{
    return new inst::int_literal(value.get_si());
}

inst::float_literal const* float_literal::inst(inst::scope const* scope) const
{
    return new inst::float_literal(value.get_d());
}

inst::reference const* reference::inst(inst::scope const* scope) const
{
    return new inst::reference(scope->query_var(pos, name));
}

inst::call const* call::inst(inst::scope const* scope) const
{
    std::vector<util::sptr<inst::expr_base const>> arg_instances;
    std::vector<inst::type const*> arg_types;
    std::for_each(args.begin()
                , args.end()
                , [&](expr_base const* expr)
                  {
                      inst::expr_base const* arg = expr->inst(scope);
                      arg_instances.push_back(std::move(util::mkptr(arg)));
                      arg_types.push_back(arg->typeof());
                  });
    return new inst::call(func->inst(pos, scope, arg_types), std::move(arg_instances));
}

inst::binary_op const* binary_op::inst(inst::scope const* scope) const
{
    inst::expr_base const* left = lhs->inst(scope);
    inst::expr_base const* right = rhs->inst(scope);
    return new inst::binary_op(std::move(util::mkptr(left))
                             , inst::operation::query_binary(pos, op, left->typeof(), right->typeof())
                             , std::move(util::mkptr(right)));
}

inst::pre_unary_op const* pre_unary_op::inst(inst::scope const* scope) const
{
    inst::expr_base const* right = rhs->inst(scope);
    return new inst::pre_unary_op(inst::operation::query_pre_unary(pos, op, right->typeof())
                                , std::move(util::mkptr(right)));
}

inst::conjunction const* conjunction::inst(inst::scope const* scope) const
{
    return new inst::conjunction(std::move(util::mkptr(lhs->inst(scope)))
                               , std::move(util::mkptr(rhs->inst(scope))));
}

inst::disjunction const* disjunction::inst(inst::scope const* scope) const
{
    return new inst::disjunction(std::move(util::mkptr(lhs->inst(scope)))
                               , std::move(util::mkptr(rhs->inst(scope))));
}

inst::negation const* negation::inst(inst::scope const* scope) const
{
    return new inst::negation(std::move(util::mkptr(rhs->inst(scope))));
}
