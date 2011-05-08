#include "stmt-nodes.h"
#include "function.h"
#include "func-inst-draft.h"
#include "symbol-table.h"
#include "../instance/stmt-nodes.h"
#include "../instance/type.h"

using namespace proto;

void Branch::addTo(util::sref<FuncInstDraft> func)
{
    _consequence_stmt->addTo(func);
    _alternative_stmt->addTo(func);
}

util::sptr<inst::Statement const> Branch::inst(util::sref<FuncInstDraft> func
                                             , util::sref<SymbolTable> st)
{
    return util::mkptr(new inst::Branch(pos
                                      , predicate->inst(st)
                                      , _consequence_stmt->inst(func, st)
                                      , _alternative_stmt->inst(func, st)));
}

void Branch::mediateInst(util::sref<FuncInstDraft> func, util::sref<SymbolTable>)
{
    _consequence_stmt->addTo(func);
    _alternative_stmt->addTo(func);
}

void DirectInst::addTo(util::sref<FuncInstDraft>) {}

util::sptr<inst::Statement const> DirectInst::inst(util::sref<FuncInstDraft> func
                                                 , util::sref<SymbolTable> st)
{
    mediateInst(func, st);
    return std::move(_result_stmt_or_nul_if_not_inst);
}

void DirectInst::mediateInst(util::sref<FuncInstDraft> func, util::sref<SymbolTable> st)
{
    if (_result_stmt_or_nul_if_not_inst.nul()) {
        _result_stmt_or_nul_if_not_inst = _inst(func, st);
    }
}

util::sptr<inst::Statement const> Arithmetics::_inst(util::sref<FuncInstDraft>
                                                   , util::sref<SymbolTable> st) const
{
    return util::mkptr(new inst::Arithmetics(expr->inst(st)));
}

util::sptr<inst::Statement const> VarDef::_inst(util::sref<FuncInstDraft>
                                              , util::sref<SymbolTable> st) const
{
    util::sptr<inst::Expression const> init_val = init->inst(st);
    util::sref<inst::Type const> init_type = init_val->typeof();
    return util::mkptr(new inst::Initialization(st->defVar(pos, init_type, name).stack_offset
                                              , std::move(init_val)));
}

util::sptr<inst::Statement const> Return::_inst(util::sref<FuncInstDraft> func
                                              , util::sref<SymbolTable> st) const
{
    util::sptr<inst::Expression const> e = ret_val->inst(st);
    func->setReturnType(pos, e->typeof());
    return util::mkptr(new inst::Return(std::move(e)));
}

util::sptr<inst::Statement const> ReturnNothing::_inst(util::sref<FuncInstDraft> func
                                                     , util::sref<SymbolTable>) const
{
    func->setReturnType(pos, inst::Type::BIT_VOID);
    return util::mkptr(new inst::ReturnNothing);
}
