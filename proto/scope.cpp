#include "scope.h"
#include "expr-nodes.h"
#include "stmt-nodes.h"

using namespace proto;

util::sptr<Expression const> Scope::makeBool(misc::position const& pos, bool value) const
{
    return std::move(util::mkptr(new BoolLiteral(pos, value)));
}

util::sptr<Expression const> Scope::makeInt(misc::position const& pos, mpz_class const& value) const
{
    return std::move(util::mkptr(new IntLiteral(pos, value)));
}

util::sptr<Expression const> Scope::makeFloat(misc::position const& pos, mpf_class const& value) const
{
    return std::move(util::mkptr(new FloatLiteral(pos, value)));
}

util::sptr<Expression const> Scope::makeBinary(misc::position const& pos
                                             , util::sptr<Expression const> lhs
                                             , std::string const& op
                                             , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new BinaryOp(pos, std::move(lhs), op, std::move(rhs))));
}

util::sptr<Expression const> Scope::makePreUnary(misc::position const& pos
                                                , std::string const& op
                                                , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new PreUnaryOp(pos, op, std::move(rhs))));
}

util::sptr<Expression const> Scope::makeConj(misc::position const& pos
                                           , util::sptr<Expression const> lhs
                                           , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new Conjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<Expression const> Scope::makeDisj(misc::position const& pos
                                           , util::sptr<Expression const> lhs
                                           , util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new Disjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<Expression const> Scope::makeNega(misc::position const& pos, util::sptr<Expression const> rhs) const
{
    return std::move(util::mkptr(new Negation(pos, std::move(rhs))));
}

void Scope::addStmt(util::sptr<Statement const> stmt)
{
    _block.addStmt(std::move(stmt));
}

std::list<util::sptr<Statement const>> const& Scope::getStmts() const
{
    return _block.getStmts();
}

Block Scope::deliver()
{
    return std::move(_block);
}
