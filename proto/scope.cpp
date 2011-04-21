#include "scope.h"
#include "expr-nodes.h"
#include "stmt-nodes.h"

using namespace proto;

util::sptr<Expression const> scope::make_bool(misc::pos_type const& pos, bool value) const
{
    return std::move(util::mkptr(new bool_literal(pos, value)));
}

util::sptr<Expression const> scope::make_int(misc::pos_type const& pos, mpz_class const& value) const
{
    return std::move(util::mkptr(new IntLiteral(pos, value)));
}

util::sptr<Expression const> scope::make_float(misc::pos_type const& pos, mpf_class const& value) const
{
    return std::move(util::mkptr(new float_literal(pos, value)));
}

util::sptr<Expression const> scope::make_binary(misc::pos_type const& pos
                                             , util::sptr<Expression const> lhs
                                             , std::string const& op
                                             , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new binary_op(pos, std::move(lhs), op, std::move(rhs))));
}

util::sptr<Expression const> scope::make_pre_unary(misc::pos_type const& pos
                                                , std::string const& op
                                                , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new pre_unary_op(pos, op, std::move(rhs))));
}

util::sptr<Expression const> scope::make_conj(misc::pos_type const& pos
                                           , util::sptr<Expression const> lhs
                                           , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new conjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<Expression const> scope::make_disj(misc::pos_type const& pos
                                           , util::sptr<Expression const> lhs
                                           , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new disjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<Expression const> scope::make_nega(misc::pos_type const& pos, util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new negation(pos, std::move(rhs))));
}

void scope::add_stmt(util::sptr<Statement const> stmt)
{
    _block.add_stmt(std::move(stmt));
}

std::list<util::sptr<Statement const>> const& scope::get_stmts() const
{
    return _block.get_stmts();
}

block scope::deliver()
{
    return std::move(_block);
}
