#include <algorithm>

#include "stmt-nodes.h"
#include "../instance/symbol-table.h"

using namespace proto;

inst::block const* block::inst(inst::scope const* scope) const
{
    inst::block* b = new inst::block;
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](stmt_base const* stmt)
                  {
                      b->add_stmt(std::move(util::mkptr(stmt->inst(scope))));
                  });
    return b;
}

void block::_set_term_status(termination_status status)
{
    if (RETURN_VOID == status) {
        _tstatus = RETURN_VOID;
    } else if (RETURN_VOID != _tstatus && RETURN_NO_VOID == status) {
        _tstatus = RETURN_NO_VOID;
    }
}

void block::add_stmt(stmt_base const* stmt)
{
    if (NULL == stmt) {
        return;
    }
    _set_term_status(stmt->termination());
    _stmts.push_back(stmt);
}

inst::arithmetics const* arithmetics::inst(inst::scope const* scope) const
{
    return new inst::arithmetics(std::move(util::mkptr(expr->inst(scope))));
}

inst::initialization const* var_def::inst(inst::scope const* scope) const
{
    inst::expr_base const* init_val = init->inst(scope);
    return new inst::initialization(scope->def_var(pos, init_val->typeof(), name).stack_offset
                                  , std::move(util::mkptr(init_val)));
}

inst::branch const* branch::inst(inst::scope const* scope) const
{
    return new inst::branch(std::move(util::mkptr(condition->inst(scope)))
                          , std::move(util::mkptr(valid->inst(scope)))
                          , std::move(util::mkptr(invalid->inst(scope))));
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

inst::loop const* loop::inst(inst::scope const* scope) const
{
    return new inst::loop(std::move(util::mkptr(condition->inst(scope)))
                        , std::move(util::mkptr(body->inst(scope))));
}

inst::func_ret const* func_ret::inst(inst::scope const* scope) const
{
    inst::expr_base const* e = ret_val->inst(scope);
    scope->set_return_type(pos, e->typeof());
    return new inst::func_ret(std::move(util::mkptr(e)));
}

inst::func_ret_nothing const* func_ret_nothing::inst(inst::scope const* scope) const
{
    scope->set_return_type(pos, inst::type::BIT_VOID);
    return new inst::func_ret_nothing;
}
