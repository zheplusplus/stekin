#include "scope.h"
#include "expr-nodes.h"
#include "stmt-nodes.h"

using namespace proto;

util::sptr<Expression const> scope::make_bool(misc::position const& pos, bool value) const
{
    return std::move(util::mkptr(new BoolLiteral(pos, value)));
}

util::sptr<Expression const> scope::make_int(misc::position const& pos, mpz_class const& value) const
{
    return std::move(util::mkptr(new IntLiteral(pos, value)));
}

util::sptr<Expression const> scope::make_float(misc::position const& pos, mpf_class const& value) const
{
    return std::move(util::mkptr(new FloatLiteral(pos, value)));
}

util::sptr<Expression const> scope::make_binary(misc::position const& pos
                                             , util::sptr<Expression const> lhs
                                             , std::string const& op
                                             , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new BinaryOp(pos, std::move(lhs), op, std::move(rhs))));
}

util::sptr<Expression const> scope::make_pre_unary(misc::position const& pos
                                                , std::string const& op
                                                , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new PreUnaryOp(pos, op, std::move(rhs))));
}

util::sptr<Expression const> scope::make_conj(misc::position const& pos
                                           , util::sptr<Expression const> lhs
                                           , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new Conjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<Expression const> scope::make_disj(misc::position const& pos
                                           , util::sptr<Expression const> lhs
                                           , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new Disjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<Expression const> scope::make_nega(misc::position const& pos, util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new Negation(pos, std::move(rhs))));
}

void scope::add_stmt(util::sptr<Statement const> stmt)
{
    _block.add_stmt(std::move(stmt));
}

std::list<util::sptr<Statement const>> const& scope::get_stmts() const
{
    return _block.get_stmts();
}

Block scope::deliver()
{
    return std::move(_block);
}
