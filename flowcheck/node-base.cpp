#include "node-base.h"
#include "expr-nodes.h"

using namespace flchk;

bool Expression::is_literal() const
{
    return false;
}

bool Expression::bool_value() const
{
    return false;
}

util::sptr<Expression const> Expression::operate(misc::pos_type const& op_pos
                                             , std::string const& op_img
                                             , mpz_class const& rhs) const
{
    return std::move(util::mkptr(new binary_op(op_pos
                                             , std::move(fold())
                                             , op_img
                                             , std::move(util::mkptr(new IntLiteral(op_pos, rhs))))));
}

util::sptr<Expression const> Expression::operate(misc::pos_type const& op_pos
                                             , std::string const& op_img
                                             , mpf_class const& rhs) const
{
    return std::move(util::mkptr(new binary_op(op_pos
                                             , std::move(fold())
                                             , op_img
                                             , std::move(util::mkptr(new float_literal(op_pos, rhs))))));
}

util::sptr<Expression const> Expression::operate(misc::pos_type const& op_pos
                                             , std::string const& op_img
                                             , bool rhs) const
{
    return std::move(util::mkptr(new binary_op(op_pos
                                             , std::move(fold())
                                             , op_img
                                             , std::move(util::mkptr(new bool_literal(op_pos, rhs))))));
}

util::sptr<Expression const> Expression::as_rhs(misc::pos_type const& op_pos
                                            , std::string const& op_img
                                            , util::sptr<Expression const> lhs) const
{
    return std::move(util::mkptr(new binary_op(op_pos, std::move(lhs), op_img, std::move(fold()))));
}

util::sptr<Expression const> Expression::as_rhs(misc::pos_type const& op_pos, std::string const& op_img) const
{
    return std::move(util::mkptr(new pre_unary_op(op_pos, op_img, std::move(fold()))));
}
