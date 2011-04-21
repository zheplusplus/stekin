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
    lhs->typeof()->check_condition_type(p);
    rhs->typeof()->check_condition_type(p);
}

Disjunction::Disjunction(misc::position const& p, util::sptr<Expression const> l, util::sptr<Expression const> r)
    : lhs(std::move(l))
    , rhs(std::move(r))
{
    lhs->typeof()->check_condition_type(p);
    rhs->typeof()->check_condition_type(p);
}

Negation::Negation(misc::position const& p, util::sptr<Expression const> r)
    : rhs(std::move(r))
{
    rhs->typeof()->check_condition_type(p);
}

util::sref<type const> IntLiteral::typeof() const
{
    return type::BIT_INT;
}

util::sref<type const> FloatLiteral::typeof() const
{
    return type::BIT_FLOAT;
}

util::sref<type const> BoolLiteral::typeof() const
{
    return type::BIT_BOOL;
}

util::sref<type const> reference::typeof() const
{
    return var.vtype;
}

util::sref<type const> call::typeof() const
{
    return func->get_return_type();
}

util::sref<type const> FuncReference::typeof() const
{
    return util::mkref(_type);
}

util::sref<type const> BinaryOp::typeof() const
{
    return op->ret_type;
}

util::sref<type const> PreUnaryOp::typeof() const
{
    return op->ret_type;
}

util::sref<type const> Conjunction::typeof() const
{
    return type::BIT_BOOL;
}

util::sref<type const> Disjunction::typeof() const
{
    return type::BIT_BOOL;
}

util::sref<type const> Negation::typeof() const
{
    return type::BIT_BOOL;
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

void reference::write() const
{
    output::ref_level(var.stack_offset, var.level, typeof()->exported_name());
}

void call::write() const
{
    output::write_call_begin(func.id());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& expr)
                  {
                       output::write_arg_seperator();
                       expr->write();
                  });
    output::write_call_end();
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
