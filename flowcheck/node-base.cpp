#include "node-base.h"
#include "expr-nodes.h"

using namespace flchk;

bool expr_base::is_literal() const
{
    return false;
}

bool expr_base::bool_value() const
{
    return false;
}

util::sptr<expr_base const> expr_base::operate(misc::pos_type const& op_pos
                                             , std::string const& op_img
                                             , mpz_class const& rhs) const
{
    return std::move(util::mkptr(new binary_op(op_pos
                                             , std::move(fold())
                                             , op_img
                                             , std::move(util::mkptr(new int_literal(op_pos, rhs))))));
}

util::sptr<expr_base const> expr_base::operate(misc::pos_type const& op_pos
                                             , std::string const& op_img
                                             , mpf_class const& rhs) const
{
    return std::move(util::mkptr(new binary_op(op_pos
                                             , std::move(fold())
                                             , op_img
                                             , std::move(util::mkptr(new float_literal(op_pos, rhs))))));
}

util::sptr<expr_base const> expr_base::operate(misc::pos_type const& op_pos
                                             , std::string const& op_img
                                             , bool rhs) const
{
    return std::move(util::mkptr(new binary_op(op_pos
                                             , std::move(fold())
                                             , op_img
                                             , std::move(util::mkptr(new bool_literal(op_pos, rhs))))));
}

util::sptr<expr_base const> expr_base::as_rhs(misc::pos_type const& op_pos
                                            , std::string const& op_img
                                            , util::sptr<expr_base const> lhs) const
{
    return std::move(util::mkptr(new binary_op(op_pos, std::move(lhs), op_img, std::move(fold()))));
}

util::sptr<expr_base const> expr_base::as_rhs(misc::pos_type const& op_pos, std::string const& op_img) const
{
    return std::move(util::mkptr(new pre_unary_op(op_pos, op_img, std::move(fold()))));
}
