#include "stmt-nodes.h"
#include "../instance/symbol-table.h"

using namespace proto;

util::sptr<inst::stmt_base const> arithmetics::inst(util::sref<inst::scope const> scope) const
{
    return std::move(util::mkptr(new inst::arithmetics(std::move(expr->inst(scope)))));
}

util::sptr<inst::stmt_base const> var_def::inst(util::sref<inst::scope const> scope) const
{
    util::sptr<inst::expr_base const> init_val = init->inst(scope);
    inst::type const* init_type = init_val->typeof();
    return std::move(util::mkptr(new inst::initialization(scope->def_var(pos, init_type, name).stack_offset
                                                        , std::move(init_val))));
}

util::sptr<inst::stmt_base const> branch::inst(util::sref<inst::scope const> scope) const
{
    return std::move(util::mkptr(new inst::branch(std::move(condition->inst(scope))
                                                , std::move(valid->inst(scope))
                                                , std::move(invalid->inst(scope)))));
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
