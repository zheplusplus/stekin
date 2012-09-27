#include <algorithm>

#include <instance/stmt-nodes.h>
#include <util/vector-append.h>

#include "stmt-nodes.h"
#include "func-inst-draft.h"
#include "symbol-table.h"
#include "type.h"
#include "variable.h"

using namespace proto;

void Branch::addTo(util::sref<FuncInstDraft> func)
{
    _consequence_stmt->addTo(func);
    _alternative_stmt->addTo(func);
}

util::sptr<inst::Statement const> Branch::inst(util::sref<FuncInstDraft> func, misc::trace& trace)
{
    predicate->type(func->getSymbols(), trace)->checkCondType(pos);
    return util::mkptr(new inst::Branch(func->level()
                                      , predicate->inst(func->getSymbols(), trace)
                                      , _consequence_stmt->inst(func, trace)
                                      , _alternative_stmt->inst(func, trace)));
}

void Branch::mediateInst(util::sref<FuncInstDraft> func, misc::trace&)
{
    _consequence_stmt->addTo(func);
    _alternative_stmt->addTo(func);
}

std::vector<util::sptr<inst::Function const>> Branch::deliverFuncs()
{
    return util::ptrs_append(_consequence_stmt->deliverFuncs(), _alternative_stmt->deliverFuncs());
}

void DirectInst::addTo(util::sref<FuncInstDraft>) {}

util::sptr<inst::Statement const> DirectInst::inst(util::sref<FuncInstDraft> func
                                                 , misc::trace& trace)
{
    mediateInst(func, trace);
    return std::move(_result_stmt_or_nul_if_not_inst);
}

void DirectInst::mediateInst(util::sref<FuncInstDraft> func, misc::trace& trace)
{
    if (_result_stmt_or_nul_if_not_inst.nul()) {
        _result_stmt_or_nul_if_not_inst = _inst(func, trace);
    }
}

std::vector<util::sptr<inst::Function const>> DirectInst::deliverFuncs()
{
    return std::vector<util::sptr<inst::Function const>>();
}

util::sptr<inst::Statement const> Arithmetics::_inst(util::sref<FuncInstDraft> func
                                                   , misc::trace& trace) const
{
    return util::mkptr(new inst::Arithmetics(func->level(), expr->inst(func->getSymbols(), trace)));
}

util::sptr<inst::Statement const> VarDef::_inst(util::sref<FuncInstDraft> func
                                              , misc::trace& trace) const
{
    util::sref<Type const> type(init->type(func->getSymbols(), trace));
    util::sptr<inst::Expression const> value(init->inst(func->getSymbols(), trace));
    int offset = func->getSymbols()->defVar(pos, type, name).stack_offset;
    return util::mkptr(new inst::Initialization(func->level()
                                              , offset
                                              , std::move(value)
                                              , type->makeInstType()));
}

util::sptr<inst::Statement const> Return::_inst(util::sref<FuncInstDraft> func
                                              , misc::trace& trace) const
{
    func->setReturnType(ret_val->type(func->getSymbols(), trace), trace);
    return util::mkptr(new inst::Return(func->level(), ret_val->inst(func->getSymbols(), trace)));
}

util::sptr<inst::Statement const> ReturnNothing::_inst(util::sref<FuncInstDraft> func
                                                     , misc::trace& trace) const
{
    func->setReturnType(Type::s_void(), trace);
    return util::mkptr(new inst::ReturnNothing);
}
