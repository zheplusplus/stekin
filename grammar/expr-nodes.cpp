#include <algorithm>

#include "expr-nodes.h"
#include "../flowcheck/filter.h"
#include "../flowcheck/function.h"
#include "../flowcheck/expr-nodes.h"
#include "../proto/node-base.h"

using namespace grammar;

util::sptr<flchk::Expression const> PreUnaryOp::compile(util::sref<flchk::Filter> filter) const
{
    return std::move(util::mkptr(new flchk::PreUnaryOp(pos
                                                     , op_img
                                                     , std::move(rhs->compile(filter)))));
}

util::sptr<flchk::Expression const> BinaryOp::compile(util::sref<flchk::Filter> filter) const
{
    return std::move(util::mkptr(new flchk::BinaryOp(pos
                                                   , std::move(lhs->compile(filter))
                                                   , op_img
                                                   , std::move(rhs->compile(filter)))));
}

util::sptr<flchk::Expression const> Conjunction::compile(util::sref<flchk::Filter> filter) const
{
    return std::move(util::mkptr(new flchk::Conjunction(pos
                                                      , lhs->compile(filter)
                                                      , rhs->compile(filter))));
}

util::sptr<flchk::Expression const> Disjunction::compile(util::sref<flchk::Filter> filter) const
{
    return std::move(util::mkptr(new flchk::Disjunction(pos
                                                      , lhs->compile(filter)
                                                      , rhs->compile(filter))));
}

util::sptr<flchk::Expression const> Negation::compile(util::sref<flchk::Filter> filter) const
{
    return std::move(util::mkptr(new flchk::Negation(pos, rhs->compile(filter))));
}

util::sptr<flchk::Expression const> Reference::compile(util::sref<flchk::Filter> filter) const
{
    return std::move(util::mkptr(new flchk::Reference(pos, filter->getSymbols(), name)));
}

util::sptr<flchk::Expression const> BoolLiteral::compile(util::sref<flchk::Filter>) const
{
    return std::move(util::mkptr(new flchk::BoolLiteral(pos, value)));
}

util::sptr<flchk::Expression const> IntLiteral::compile(util::sref<flchk::Filter>) const
{
    return std::move(util::mkptr(new flchk::IntLiteral(pos, value)));
}

util::sptr<flchk::Expression const> FloatLiteral::compile(util::sref<flchk::Filter>) const
{
    return std::move(util::mkptr(new flchk::FloatLiteral(pos, value)));
}

util::sptr<flchk::Expression const> Call::compile(util::sref<flchk::Filter> filter) const
{
    std::vector<util::sptr<flchk::Expression const>> arguments;
    arguments.reserve(args.size());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& expr)
                  {
                      arguments.push_back(expr->compile(filter));
                  });
    return std::move(util::mkptr(new flchk::Call(pos
                                               , filter->getSymbols()
                                               , name
                                               , std::move(arguments))));
}

util::sptr<flchk::Expression const> FuncReference::compile(util::sref<flchk::Filter> filter) const
{
    return util::mkptr(new flchk::FuncReference(pos, filter->getSymbols(), name, param_count));
}
