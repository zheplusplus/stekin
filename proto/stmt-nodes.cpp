#include "stmt-nodes.h"
#include "inst-mediates.h"
#include "../instance/stmt-nodes.h"
#include "../instance/scope.h"
#include "../instance/type.h"
#include "../instance/symbol-table.h"

using namespace proto;

static util::sptr<inst::mediate_base> mkdirect(inst::stmt_base* stmt)
{
    return std::move(util::mkmptr(new direct_inst(std::move(util::mkptr(stmt)))));
}

util::sptr<inst::mediate_base> arithmetics::inst(util::sref<inst::scope> scope) const
{
    return std::move(mkdirect(new inst::arithmetics(std::move(expr->inst(scope)))));
}

util::sptr<inst::mediate_base> var_def::inst(util::sref<inst::scope> scope) const
{
    util::sptr<inst::expr_base const> init_val = init->inst(scope);
    inst::type const* init_type = init_val->typeof();
    return std::move(mkdirect(new inst::initialization(scope->def_var(pos, init_type, name).stack_offset
                                                     , std::move(init_val))));
}

util::sptr<inst::mediate_base> branch::inst(util::sref<inst::scope> scope) const
{
    return std::move(util::mkmptr(new branch_mediate(pos
                                                   , std::move(_predicate->inst(scope))
                                                   , _consequence.get_stmts()
                                                   , _alternative.get_stmts()
                                                   , scope)));
}

util::sptr<inst::mediate_base> func_ret::inst(util::sref<inst::scope> scope) const
{
    util::sptr<inst::expr_base const> e = ret_val->inst(scope);
    scope->set_return_type(pos, e->typeof());
    return std::move(mkdirect(new inst::func_ret(std::move(e))));
}

util::sptr<inst::mediate_base> func_ret_nothing::inst(util::sref<inst::scope> scope) const
{
    scope->set_return_type(pos, inst::type::BIT_VOID);
    return std::move(mkdirect(new inst::func_ret_nothing));
}
