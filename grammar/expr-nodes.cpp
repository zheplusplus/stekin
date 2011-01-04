#include <algorithm>

#include "expr-nodes.h"

using namespace grammar;

proto::expr_prototype const* pre_unary_oper::compile(proto::scope const* scope) const
{
    return scope->make_pre_unary(pos, op_img, rhs->compile(scope));
}

proto::expr_prototype const* binary_oper::compile(proto::scope const* scope) const
{
    return scope->make_binary(pos, lhs->compile(scope), op_img, rhs->compile(scope));
}

proto::expr_prototype const* conjunction::compile(proto::scope const* scope) const
{
    return scope->make_conj(pos, lhs->compile(scope), rhs->compile(scope));
}

proto::expr_prototype const* disjunction::compile(proto::scope const* scope) const
{
    return scope->make_disj(pos, lhs->compile(scope), rhs->compile(scope));
}

proto::expr_prototype const* negation::compile(proto::scope const* scope) const
{
    return scope->make_nega(pos, rhs->compile(scope));
}

proto::expr_prototype const* reference::compile(proto::scope const* scope) const
{
    return scope->reference(pos, name);
}

proto::expr_prototype const* int_literal::compile(proto::scope const* scope) const
{
    return scope->make_int(pos, value);
}

proto::expr_prototype const* float_literal::compile(proto::scope const* scope) const
{
    return scope->make_float(pos, value);
}

proto::expr_prototype const* call::compile(proto::scope const* scope) const
{
    std::vector<proto::expr_prototype const*> arguments;
    arguments.reserve(args.size());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<expr_base const> const& expr)
                  {
                      arguments.push_back(expr->compile(scope));
                  });
    return scope->call(pos, name, arguments);
}
