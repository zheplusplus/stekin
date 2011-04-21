#include <algorithm>

#include "expr-nodes.h"
#include "function.h"
#include "../instance/expr-nodes.h"
#include "../instance/scope.h"

using namespace proto;

util::sptr<inst::Expression const> BoolLiteral::inst(util::sref<inst::scope>) const
{
    return std::move(util::mkptr(new inst::BoolLiteral(value)));
}

util::sptr<inst::Expression const> IntLiteral::inst(util::sref<inst::scope>) const
{
    return std::move(util::mkptr(new inst::IntLiteral(value.get_si())));
}

util::sptr<inst::Expression const> FloatLiteral::inst(util::sref<inst::scope>) const
{
    return std::move(util::mkptr(new inst::FloatLiteral(value.get_d())));
}

util::sptr<inst::Expression const> reference::inst(util::sref<inst::scope> scope) const
{
    return std::move(util::mkptr(new inst::reference(scope->query_var(pos, name))));
}

template <typename _CallMaker>
static util::sptr<inst::Expression const> instantiate_Function(
            std::vector<util::sptr<Expression const>> const& args
          , util::sref<inst::scope> scope
          , _CallMaker call_maker)
{
    std::vector<util::sptr<inst::Expression const>> arg_instances;
    std::vector<util::sref<inst::type const>> arg_types;
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& expr)
                  {
                      util::sptr<inst::Expression const> arg(std::move(expr->inst(scope)));
                      arg_types.push_back(arg->typeof());
                      arg_instances.push_back(std::move(arg));
                  });
    return std::move(call_maker(arg_types, std::move(arg_instances)));
}

util::sptr<inst::Expression const> call::inst(util::sref<inst::scope> scope) const
{
    return std::move(instantiate_Function(
                args
              , scope
              , [&](std::vector<util::sref<inst::type const>> const& arg_types
                  , std::vector<util::sptr<inst::Expression const>> args)
                {
                    return std::move(util::mkptr(new inst::call(func->inst(pos, scope, arg_types)
                                                              , std::move(args))));
                }));
}

util::sptr<inst::Expression const> functor::inst(util::sref<inst::scope> scope) const
{
    return std::move(instantiate_Function(
                args
              , scope
              , [&](std::vector<util::sref<inst::type const>> const& arg_types
                  , std::vector<util::sptr<inst::Expression const>> args)
                {
                    return std::move(scope->query_var(pos, name).call_func(pos, arg_types, std::move(args)));
                }));
}

util::sptr<inst::Expression const> func_reference::inst(util::sref<inst::scope> scope) const
{
    return std::move(util::mkptr(new inst::func_reference(pos   
                                                        , func
                                                        , scope->level()
                                                        , func->bind_external_vars(pos, scope))));
}

util::sptr<inst::Expression const> binary_op::inst(util::sref<inst::scope> scope) const
{
    util::sptr<inst::Expression const> left = lhs->inst(scope);
    util::sptr<inst::Expression const> right = rhs->inst(scope);
    util::sref<inst::type const> ltype = left->typeof();
    util::sref<inst::type const> rtype = right->typeof();
    return std::move(util::mkptr(new inst::binary_op(std::move(left)
                                                   , scope->query_binary(pos, op, ltype, rtype)
                                                   , std::move(right))));
}

util::sptr<inst::Expression const> pre_unary_op::inst(util::sref<inst::scope> scope) const
{
    util::sptr<inst::Expression const> right = rhs->inst(scope);
    util::sref<inst::type const> rtype = right->typeof();
    return std::move(util::mkptr(
                new inst::pre_unary_op(scope->query_pre_unary(pos, op, rtype), std::move(right))));
}

util::sptr<inst::Expression const> conjunction::inst(util::sref<inst::scope> scope) const
{
    return std::move(util::mkptr(new inst::conjunction(pos
                                                     , std::move(lhs->inst(scope))
                                                     , std::move(rhs->inst(scope)))));
}

util::sptr<inst::Expression const> disjunction::inst(util::sref<inst::scope> scope) const
{
    return std::move(util::mkptr(new inst::disjunction(pos
                                                     , std::move(lhs->inst(scope))
                                                     , std::move(rhs->inst(scope)))));
}

util::sptr<inst::Expression const> negation::inst(util::sref<inst::scope> scope) const
{
    return std::move(util::mkptr(new inst::negation(pos, std::move(rhs->inst(scope)))));
}
