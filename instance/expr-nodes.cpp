#include <algorithm>

#include "expr-nodes.h"
#include "function.h"
#include "operation.h"
#include "../output/func-writer.h"
#include "../output/statement-writer.h"

using namespace inst;

Conjunction::Conjunction(misc::position const& p, util::sptr<Expression const> l, util::sptr<Expression const> r)
    : lhs(std::move(l))
    , rhs(std::move(r))
{
    lhs->typeof()->checkCondType(p);
    rhs->typeof()->checkCondType(p);
}

Disjunction::Disjunction(misc::position const& p, util::sptr<Expression const> l, util::sptr<Expression const> r)
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
    return func->get_return_type();
}

util::sref<Type const> FuncReference::typeof() const
{
    return util::mkref(_type);
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
    output::write_int(value);
}

void FloatLiteral::write() const
{
    output::write_float(value);
}

void BoolLiteral::write() const
{
    output::write_bool(value);
}

void Reference::write() const
{
    output::ref_level(var.stack_offset, var.level, typeof()->exportedName());
}

void Call::write() const
{
    output::writeCallBegin(func.id());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& expr)
                  {
                       output::write_arg_seperator();
                       expr->write();
                  });
    output::writeCallEnd();
}

void FuncReference::write() const
{
    _type.write();
}

void BinaryOp::write() const
{
    output::begin_expr();
    lhs->write();
    output::write_operator(op->op_img);
    rhs->write();
    output::end_expr();
}

void PreUnaryOp::write() const
{
    output::begin_expr();
    output::write_operator(op->op_img);
    rhs->write();
    output::end_expr();
}

void Conjunction::write() const
{
    output::begin_expr();
    lhs->write();
    output::write_operator("&&");
    rhs->write();
    output::end_expr();
}

void Disjunction::write() const
{
    output::begin_expr();
    lhs->write();
    output::write_operator("||");
    rhs->write();
    output::end_expr();
}

void Negation::write() const
{
    output::begin_expr();
    output::write_operator("!");
    rhs->write();
    output::end_expr();
}
