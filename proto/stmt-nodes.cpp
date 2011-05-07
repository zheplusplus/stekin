#include "stmt-nodes.h"
#include "function.h"
#include "../instance/stmt-nodes.h"
#include "../instance/scope.h"
#include "../instance/type.h"
#include "../instance/symbol-table.h"

using namespace proto;

void BranchMediate::addTo(util::sref<inst::Scope> scope)
{
    _consequence_mediate->addTo(scope);
    _alternative_mediate->addTo(scope);
}

util::sptr<inst::Statement const> BranchMediate::inst(util::sref<inst::Scope> scope
                                                    , util::sref<inst::SymbolTable> st)
{
    return util::mkptr(new inst::Branch(pos
                                      , predicate->inst(st)
                                      , _consequence_mediate->inst(scope, st)
                                      , _alternative_mediate->inst(scope, st)));
}

void BranchMediate::mediateInst(util::sref<inst::Scope> scope, util::sref<inst::SymbolTable>)
{
    _consequence_mediate->addTo(scope);
    _alternative_mediate->addTo(scope);
}

void DirectInst::addTo(util::sref<inst::Scope>) {}

util::sptr<inst::Statement const> DirectInst::inst(util::sref<inst::Scope> scope
                                                 , util::sref<inst::SymbolTable> st)
{
    mediateInst(scope, st);
    return std::move(_result_stmt_or_nul_if_not_inst);
}

void DirectInst::mediateInst(util::sref<inst::Scope> scope, util::sref<inst::SymbolTable> st)
{
    if (_result_stmt_or_nul_if_not_inst.nul()) {
        _result_stmt_or_nul_if_not_inst = _inst(scope, st);
    }
}

util::sptr<inst::Statement const> Arithmetics::_inst(util::sref<inst::Scope>
                                                   , util::sref<inst::SymbolTable> st) const
{
    return util::mkptr(new inst::Arithmetics(expr->inst(st)));
}

util::sptr<inst::Statement const> VarDef::_inst(util::sref<inst::Scope>
                                              , util::sref<inst::SymbolTable> st) const
{
    util::sptr<inst::Expression const> init_val = init->inst(st);
    util::sref<inst::Type const> init_type = init_val->typeof();
    return util::mkptr(new inst::Initialization(st->defVar(pos, init_type, name).stack_offset
                                              , std::move(init_val)));
}

util::sptr<inst::Statement const> Return::_inst(util::sref<inst::Scope> scope
                                              , util::sref<inst::SymbolTable> st) const
{
    util::sptr<inst::Expression const> e = ret_val->inst(st);
    scope->setReturnType(pos, e->typeof());
    return util::mkptr(new inst::Return(std::move(e)));
}

util::sptr<inst::Statement const> ReturnNothing::_inst(util::sref<inst::Scope> scope
                                                     , util::sref<inst::SymbolTable>) const
{
    scope->setReturnType(pos, inst::Type::BIT_VOID);
    return util::mkptr(new inst::ReturnNothing);
}
