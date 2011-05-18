#include <algorithm>

#include "stmt-nodes.h"
#include "func-inst-draft.h"
#include "symbol-table.h"
#include "type.h"
#include "variable.h"
#include "../instance/stmt-nodes.h"

using namespace proto;

void Branch::addTo(util::sref<FuncInstDraft> func)
{
    _consequence_stmt->addTo(func);
    _alternative_stmt->addTo(func);
}

util::sptr<inst::Statement const> Branch::inst(util::sref<FuncInstDraft> func
                                             , util::sref<SymbolTable> st)
{
    predicate->type(st)->checkCondType(pos);
    return util::mkptr(new inst::Branch(predicate->inst(st)
                                      , _consequence_stmt->inst(func, st)
                                      , _alternative_stmt->inst(func, st)));
}

void Branch::mediateInst(util::sref<FuncInstDraft> func, util::sref<SymbolTable>)
{
    _consequence_stmt->addTo(func);
    _alternative_stmt->addTo(func);
}

static std::vector<util::sptr<inst::Function const>>
        join(std::vector<util::sptr<inst::Function const>> a
           , std::vector<util::sptr<inst::Function const>> b)
{
    std::for_each(b.begin()
                , b.end()
                , [&](util::sptr<inst::Function const>& f)
                  {
                      a.push_back(std::move(f));
                  });
    return std::move(a);
}

std::vector<util::sptr<inst::Function const>> Branch::deliverFuncs()
{
    return join(_consequence_stmt->deliverFuncs(), _alternative_stmt->deliverFuncs());
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

std::vector<util::sptr<inst::Function const>> DirectInst::deliverFuncs()
{
    return std::vector<util::sptr<inst::Function const>>();
}

util::sptr<inst::Statement const> Arithmetics::_inst(util::sref<FuncInstDraft>
                                                   , util::sref<SymbolTable> st) const
{
    return util::mkptr(new inst::Arithmetics(expr->inst(st)));
}

util::sptr<inst::Statement const> VarDef::_inst(util::sref<FuncInstDraft>
                                              , util::sref<SymbolTable> st) const
{
    util::sref<Type const> type(init->type(st));
    return util::mkptr(new inst::Initialization(st->defVar(pos, type, name).stack_offset
                                              , init->inst(st)
                                              , type->exportedName()));
}

util::sptr<inst::Statement const> Return::_inst(util::sref<FuncInstDraft> func
                                              , util::sref<SymbolTable> st) const
{
    func->setReturnType(pos, ret_val->type(st));
    return util::mkptr(new inst::Return(ret_val->inst(st)));
}

util::sptr<inst::Statement const> ReturnNothing::_inst(util::sref<FuncInstDraft> func
                                                     , util::sref<SymbolTable>) const
{
    func->setReturnType(pos, Type::BIT_VOID);
    return util::mkptr(new inst::ReturnNothing);
}
