#include <algorithm>

#include "expr-nodes.h"
#include "function.h"
#include "operation.h"
#include "../output/func-writer.h"
#include "../output/statement-writer.h"

using namespace inst;

Conjunction::Conjunction(misc::position const& p
                       , util::sptr<Expression const> l
                       , util::sptr<Expression const> r)
    : lhs(std::move(l))
    , rhs(std::move(r))
{
    lhs->typeof()->checkCondType(p);
    rhs->typeof()->checkCondType(p);
}

Disjunction::Disjunction(misc::position const& p
                       , util::sptr<Expression const> l
                       , util::sptr<Expression const> r)
    : lhs(std::move(l))
    , rhs(std::move(r))
{
    lhs->typeof()->checkCondType(p);
    rhs->typeof()->checkCondType(p);
}

Negation::Negation(misc::position const& p, util::sptr<Expression const> r)
    : rhs(std::move(r))
{
    rhs->typeof()->checkCondType(p);
}

util::sref<Type const> IntLiteral::typeof() const
{
    return Type::BIT_INT;
}

util::sref<Type const> FloatLiteral::typeof() const
{
    return Type::BIT_FLOAT;
}

util::sref<Type const> BoolLiteral::typeof() const
{
    return Type::BIT_BOOL;
}

util::sref<Type const> Reference::typeof() const
{
    return var.type;
}

util::sref<Type const> Call::typeof() const
{
    return func->getReturnType();
}

util::sref<Type const> FuncReference::typeof() const
{
    return type;
}

util::sref<Type const> BinaryOp::typeof() const
{
    return op->ret_type;
}

util::sref<Type const> PreUnaryOp::typeof() const
{
    return op->ret_type;
}

util::sref<Type const> Conjunction::typeof() const
{
    return Type::BIT_BOOL;
}

util::sref<Type const> Disjunction::typeof() const
{
    return Type::BIT_BOOL;
}

util::sref<Type const> Negation::typeof() const
{
    return Type::BIT_BOOL;
}

void IntLiteral::write() const
{
    output::writeInt(value);
}

void FloatLiteral::write() const
{
    output::writeFloat(value);
}

void BoolLiteral::write() const
{
    output::writeBool(value);
}

void Reference::write() const
{
    output::refLevel(var.stack_offset, var.level, typeof()->exportedName());
}

void Call::write() const
{
    output::writeCallBegin(func.id());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& expr)
                  {
                       output::writeArgSeparator();
                       expr->write();
                  });
    output::writeCallEnd();
}

void FuncReference::write() const
{
    type->write();
}

void BinaryOp::write() const
{
    output::beginExpr();
    lhs->write();
    output::writeOperator(op->op_img);
    rhs->write();
    output::endExpr();
}

void PreUnaryOp::write() const
{
    output::beginExpr();
    output::writeOperator(op->op_img);
    rhs->write();
    output::endExpr();
}

void Conjunction::write() const
{
    output::beginExpr();
    lhs->write();
    output::writeOperator("&&");
    rhs->write();
    output::endExpr();
}

void Disjunction::write() const
{
    output::beginExpr();
    lhs->write();
    output::writeOperator("||");
    rhs->write();
    output::endExpr();
}

void Negation::write() const
{
    output::beginExpr();
    output::writeOperator("!");
    rhs->write();
    output::endExpr();
}
