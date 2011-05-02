#include "stmt-nodes.h"
#include "inst-mediates.h"
#include "function.h"
#include "../instance/stmt-nodes.h"
#include "../instance/scope.h"
#include "../instance/type.h"
#include "../instance/symbol-table.h"

using namespace proto;

static util::sptr<inst::MediateBase> mkdirect(inst::Statement* stmt)
{
    return std::move(util::mkmptr(new DirectInst(std::move(util::mkptr(stmt)))));
}

util::sptr<inst::MediateBase> Arithmetics::inst(util::sref<inst::Scope> scope) const
{
    return std::move(mkdirect(new inst::Arithmetics(std::move(expr->inst(scope)))));
}

util::sptr<inst::MediateBase> VarDef::inst(util::sref<inst::Scope> scope) const
{
    util::sptr<inst::Expression const> init_val = init->inst(scope);
    util::sref<inst::Type const> init_type = init_val->typeof();
    return std::move(mkdirect(new inst::Initialization(scope->defVar(pos, init_type, name)
                                                                .stack_offset
                                                     , std::move(init_val))));
}

util::sptr<inst::MediateBase> Branch::inst(util::sref<inst::Scope> scope) const
{
    return std::move(util::mkmptr(new BranchMediate(pos
                                                  , std::move(_predicate->inst(scope))
                                                  , _consequence.getStmts()
                                                  , _alternative.getStmts()
                                                  , scope)));
}

util::sptr<inst::MediateBase> Return::inst(util::sref<inst::Scope> scope) const
{
    util::sptr<inst::Expression const> e = ret_val->inst(scope);
    scope->setReturnType(pos, e->typeof());
    return std::move(mkdirect(new inst::Return(std::move(e))));
}

util::sptr<inst::MediateBase> ReturnNothing::inst(util::sref<inst::Scope> scope) const
{
    scope->setReturnType(pos, inst::Type::BIT_VOID);
    return std::move(mkdirect(new inst::ReturnNothing));
}
