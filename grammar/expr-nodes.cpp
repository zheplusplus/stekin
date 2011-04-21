#include <algorithm>

#include "expr-nodes.h"
#include "../flowcheck/expr-nodes.h"

using namespace grammar;

util::sptr<flchk::Expression const> pre_unary_op::compile() const
{
    return std::move(util::mkptr(new flchk::pre_unary_op(pos, op_img, std::move(rhs->compile()))));
}

util::sptr<flchk::Expression const> binary_op::compile() const
{
    return std::move(util::mkptr(new flchk::binary_op(pos
                                                    , std::move(lhs->compile())
                                                    , op_img
                                                    , std::move(rhs->compile()))));
}

util::sptr<flchk::Expression const> conjunction::compile() const
{
    return std::move(util::mkptr(new flchk::conjunction(pos, lhs->compile(), rhs->compile())));
}

util::sptr<flchk::Expression const> disjunction::compile() const
{
    return std::move(util::mkptr(new flchk::disjunction(pos, lhs->compile(), rhs->compile())));
}

util::sptr<flchk::Expression const> negation::compile() const
{
    return std::move(util::mkptr(new flchk::negation(pos, rhs->compile())));
}

util::sptr<flchk::Expression const> reference::compile() const
{
    return std::move(util::mkptr(new flchk::reference(pos, name)));
}

util::sptr<flchk::Expression const> bool_literal::compile() const
{
    return std::move(util::mkptr(new flchk::bool_literal(pos, value)));
}

util::sptr<flchk::Expression const> IntLiteral::compile() const
{
    return std::move(util::mkptr(new flchk::IntLiteral(pos, value)));
}

util::sptr<flchk::Expression const> float_literal::compile() const
{
    return std::move(util::mkptr(new flchk::float_literal(pos, value)));
}

util::sptr<flchk::Expression const> call::compile() const
{
    std::vector<util::sptr<flchk::Expression const>> arguments;
    arguments.reserve(args.size());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& expr)
                  {
                      arguments.push_back(expr->compile());
                  });
    return std::move(util::mkptr(new flchk::call(pos, name, std::move(arguments))));
}

util::sptr<flchk::Expression const> func_reference::compile() const
{
    return std::move(util::mkptr(new flchk::func_reference(pos, name, param_count)));
}
