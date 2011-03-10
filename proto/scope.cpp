#include "scope.h"
#include "expr-nodes.h"
#include "stmt-nodes.h"

using namespace proto;

util::sptr<expr_base const> scope::make_bool(misc::pos_type const& pos, bool value) const
{
    return std::move(util::mkptr(new bool_literal(pos, value)));
}

util::sptr<expr_base const> scope::make_int(misc::pos_type const& pos, std::string const& value) const
{
    return std::move(util::mkptr(new int_literal(pos, value)));
}

util::sptr<expr_base const> scope::make_float(misc::pos_type const& pos, std::string const& value) const
{
    return std::move(util::mkptr(new float_literal(pos, value)));
}

util::sptr<expr_base const> scope::make_binary(misc::pos_type const& pos
                                             , util::sptr<expr_base const> lhs
                                             , std::string const& op
                                             , util::sptr<expr_base const> rhs) const
{
    return std::move(util::mkptr(new binary_op(pos, std::move(lhs), op, std::move(rhs))));
}

util::sptr<expr_base const> scope::make_pre_unary(misc::pos_type const& pos
                                                , std::string const& op
                                                , util::sptr<expr_base const> rhs) const
{
    return std::move(util::mkptr(new pre_unary_op(pos, op, std::move(rhs))));
}

util::sptr<expr_base const> scope::make_conj(misc::pos_type const& pos
                                           , util::sptr<expr_base const> lhs
                                           , util::sptr<expr_base const> rhs) const
{
    return std::move(util::mkptr(new conjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<expr_base const> scope::make_disj(misc::pos_type const& pos
                                           , util::sptr<expr_base const> lhs
                                           , util::sptr<expr_base const> rhs) const
{
    return std::move(util::mkptr(new disjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<expr_base const> scope::make_nega(misc::pos_type const& pos, util::sptr<expr_base const> rhs) const
{
    return std::move(util::mkptr(new negation(pos, std::move(rhs))));
}

void scope::add_func_ret(misc::pos_type const& pos, util::sptr<expr_base const> ret_val)
{
    _status_changed_by_return(RETURN_NO_VOID);
    _block.add_stmt(std::move(util::mkptr(new func_ret(pos, std::move(ret_val)))));
}

void scope::add_func_ret_nothing(misc::pos_type const& pos)
{
    _status_changed_by_return(RETURN_VOID);
    _block.add_stmt(std::move(util::mkptr(new func_ret_nothing(pos))));
}

void scope::add_arith(misc::pos_type const& pos, util::sptr<expr_base const> expr)
{
    _block.add_stmt(std::move(util::mkptr(new arithmetics(pos, std::move(expr)))));
}

void scope::add_branch(misc::pos_type const& pos
                     , util::sptr<expr_base const> predicate
                     , util::sptr<scope> consequence
                     , util::sptr<scope> alternative)
{
    _status_changed_by_sub_scope_status(consequence->_status);
    _status_changed_by_sub_scope_status(alternative->_status);
    _block.add_stmt(std::move(util::mkptr(new branch(pos
                                                   , std::move(predicate)
                                                   , std::move(consequence->_block)
                                                   , std::move(alternative->_block)))));
}

void scope::add_custom_statement(util::sptr<stmt_base const> stmt)
{
    _block.add_stmt(std::move(stmt));
}

termination_status scope::termination() const
{
    return _status;
}

void scope::_status_changed_by_sub_scope_status(termination_status sub_status)
{
    switch (_status) {
    case RETURN_VOID:
       return;
    case RETURN_NO_VOID:
       {
           switch (sub_status) {
           case RETURN_VOID:
           case PARTIAL_RETURN_VOID:
               _status = PARTIAL_RETURN_VOID;
               return;
           default:
               return;
           }
       }
    case PARTIAL_RETURN_VOID:
       return;
    case PARTIAL_RETURN_NO_VOID:
       {
           switch (sub_status) {
           case RETURN_VOID:
           case PARTIAL_RETURN_VOID:
               _status = PARTIAL_RETURN_VOID;
               return;
           default:
               return;
           }
       }
    default:
       {
           switch (sub_status) {
           case RETURN_VOID:
           case PARTIAL_RETURN_VOID:
               _status = PARTIAL_RETURN_VOID;
               return;
           case RETURN_NO_VOID:
           case PARTIAL_RETURN_NO_VOID:
               _status = PARTIAL_RETURN_NO_VOID;
               return;
           default:
               return;
           }
       }
    }
}

void scope::_status_changed_by_return(termination_status status)
{
    switch (status) {
    case RETURN_VOID:
        _status = RETURN_VOID;
        return;
    case RETURN_NO_VOID:
        {
            switch (_status) {
            case PARTIAL_RETURN_NO_VOID:
            case NO_EXPLICIT_TERMINATION:
                _status = RETURN_NO_VOID;
                return;
            default:
                return;
            }
        }
    default:
        return;
    }
}

std::list<util::sptr<stmt_base const>> const& scope::get_stmts() const
{
    return _block.get_stmts();
}
