#include <algorithm>

#include "expr-nodes.h"
#include "../flowcheck/expr-nodes.h"

using namespace grammar;

util::sptr<flchk::Expression const> PreUnaryOp::compile() const
{
    return std::move(util::mkptr(new flchk::PreUnaryOp(pos, op_img, std::move(rhs->compile()))));
}

util::sptr<flchk::Expression const> BinaryOp::compile() const
{
    return std::move(util::mkptr(new flchk::BinaryOp(pos
                                                   , std::move(lhs->compile())
                                                   , op_img
                                                   , std::move(rhs->compile()))));
}

util::sptr<flchk::Expression const> Conjunction::compile() const
{
    return std::move(util::mkptr(new flchk::Conjunction(pos, lhs->compile(), rhs->compile())));
}

util::sptr<flchk::Expression const> Disjunction::compile() const
{
    return std::move(util::mkptr(new flchk::Disjunction(pos, lhs->compile(), rhs->compile())));
}

util::sptr<flchk::Expression const> Negation::compile() const
{
    return std::move(util::mkptr(new flchk::Negation(pos, rhs->compile())));
}

util::sptr<flchk::Expression const> Reference::compile() const
{
    return std::move(util::mkptr(new flchk::reference(pos, name)));
}

util::sptr<flchk::Expression const> BoolLiteral::compile() const
{
    return std::move(util::mkptr(new flchk::BoolLiteral(pos, value)));
}

util::sptr<flchk::Expression const> IntLiteral::compile() const
{
    return std::move(util::mkptr(new flchk::IntLiteral(pos, value)));
}

util::sptr<flchk::Expression const> FloatLiteral::compile() const
{
    return std::move(util::mkptr(new flchk::FloatLiteral(pos, value)));
}

util::sptr<flchk::Expression const> Call::compile() const
{
    std::vector<util::sptr<flchk::Expression const>> arguments;
    arguments.reserve(args.size());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& expr)
                  {
                      arguments.push_back(expr->compile());
                  });
    return std::move(util::mkptr(new flchk::Call(pos, name, std::move(arguments))));
}

util::sptr<flchk::Expression const> FuncReference::compile() const
{
    return std::move(util::mkptr(new flchk::FuncReference(pos, name, param_count)));
}
