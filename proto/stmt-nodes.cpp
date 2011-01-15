#include <algorithm>

#include "stmt-nodes.h"
#include "../instance/symbol-table.h"

using namespace proto;

util::sptr<inst::stmt_base const> block::inst(util::sref<inst::scope const> scope) const
{
    util::sptr<inst::block> b(new inst::block);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<stmt_base const> const& stmt)
                  {
                      b->add_stmt(std::move(stmt->inst(scope)));
                  });
    return std::move(b);
}

void block::_set_term_status(termination_status status)
{
    if (RETURN_VOID == status) {
        _tstatus = RETURN_VOID;
    } else if (RETURN_VOID != _tstatus && RETURN_NO_VOID == status) {
        _tstatus = RETURN_NO_VOID;
    }
}

util::sptr<inst::stmt_base const> arithmetics::inst(util::sref<inst::scope const> scope) const
{
    return std::move(util::mkptr(new inst::arithmetics(std::move(expr->inst(scope)))));
}

util::sptr<inst::stmt_base const> var_def::inst(util::sref<inst::scope const> scope) const
{
    util::sptr<inst::expr_base const> init_val = init->inst(scope);
    return std::move(util::mkptr(
                new inst::initialization(scope->def_var(pos, init_val->typeof(), name).stack_offset
                                       , std::move(init_val))));
}

util::sptr<inst::stmt_base const> branch::inst(util::sref<inst::scope const> scope) const
{
    return std::move(util::mkptr(new inst::branch(std::move(condition->inst(scope))
                                                , std::move(valid->inst(scope))
                                                , std::move(invalid->inst(scope)))));
}

termination_status branch::termination() const
{
    if (NO_EXPLICIT_TERMINATION == valid->termination() || NO_EXPLICIT_TERMINATION == invalid->termination()) {
        return NO_EXPLICIT_TERMINATION;
    }
    if (RETURN_VOID == valid->termination() || RETURN_VOID == invalid->termination()) {
        return RETURN_VOID;
    }
    return RETURN_NO_VOID;
}

util::sptr<inst::stmt_base const> loop::inst(util::sref<inst::scope const> scope) const
{
    return std::move(util::mkptr(new inst::loop(std::move(condition->inst(scope))
                                              , std::move(body->inst(scope)))));
}

util::sptr<inst::stmt_base const> func_ret::inst(util::sref<inst::scope const> scope) const
{
    util::sptr<inst::expr_base const> e = ret_val->inst(scope);
    scope->set_return_type(pos, e->typeof());
    return std::move(util::mkptr(new inst::func_ret(std::move(e))));
}

util::sptr<inst::stmt_base const> func_ret_nothing::inst(util::sref<inst::scope const> scope) const
{
    scope->set_return_type(pos, inst::type::BIT_VOID);
    return std::move(util::mkptr(new inst::func_ret_nothing));
}
