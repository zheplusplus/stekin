#include <algorithm>

#include "expr-nodes.h"
#include "function.h"
#include "func-inst-draft.h"
#include "symbol-table.h"
#include "operation.h"
#include "../instance/expr-nodes.h"

using namespace proto;

util::sref<inst::Type const> BoolLiteral::type(util::sref<SymbolTable const>)
{
    return inst::Type::BIT_BOOL;
}

util::sptr<inst::Expression const> BoolLiteral::inst(util::sref<SymbolTable const>)
{
    return util::mkptr(new inst::BoolLiteral(value));
}

util::sref<inst::Type const> IntLiteral::type(util::sref<SymbolTable const>)
{
    return inst::Type::BIT_INT;
}

util::sptr<inst::Expression const> IntLiteral::inst(util::sref<SymbolTable const>)
{
    return util::mkptr(new inst::IntLiteral(value.get_si()));
}

util::sref<inst::Type const> FloatLiteral::type(util::sref<SymbolTable const>)
{
    return inst::Type::BIT_FLOAT;
}

util::sptr<inst::Expression const> FloatLiteral::inst(util::sref<SymbolTable const>)
{
    return util::mkptr(new inst::FloatLiteral(value.get_d()));
}

util::sref<inst::Type const> Reference::type(util::sref<SymbolTable const> st)
{
    return st->queryVar(pos, name).type;
}

util::sptr<inst::Expression const> Reference::inst(util::sref<SymbolTable const> st)
{
    Variable var(st->queryVar(pos, name));
    return util::mkptr(new inst::Reference(var.type, var.level, var.stack_offset));
}

static std::vector<util::sptr<inst::Expression const>> instForArgs(
                                            std::vector<util::sptr<Expression>> const& args
                                          , util::sref<SymbolTable const> st)
{
    std::vector<util::sptr<inst::Expression const>> arg_instances;
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression> const& arg)
                  {
                      arg_instances.push_back(arg->inst(st));
                  });
    return std::move(arg_instances);
}

static std::vector<util::sref<inst::Type const>> instForTypes(
                                            std::vector<util::sptr<Expression>> const& args
                                          , util::sref<SymbolTable const> st)
{
    std::vector<util::sref<inst::Type const>> arg_types;
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression> const& arg)
                  {
                      arg_types.push_back(arg->type(st));
                  });
    return arg_types;
}

util::sref<inst::Type const> Call::type(util::sref<SymbolTable const> st)
{
    _checkInst(st);
    return _func->inst(pos, st, instForTypes(_args, st))->getReturnType();
}

util::sptr<inst::Expression const> Call::inst(util::sref<SymbolTable const> st)
{
    _checkInst(st);
    return std::move(_result_or_nul_if_not_inst);
}

void Call::_checkInst(util::sref<SymbolTable const> st)
{
    if (_result_or_nul_if_not_inst.nul()) {
        util::sref<FuncInstDraft> draft(_func->inst(pos, st, instForTypes(_args, st)));
        _result_or_nul_if_not_inst.reset(
                new inst::Call(draft->getReturnType(), draft.id(), instForArgs(_args, st)));
    }
}

util::sref<inst::Type const> Functor::type(util::sref<SymbolTable const> st)
{
    _checkInst(st);
    return _result_or_nul_if_not_inst->typeof();
}

util::sptr<inst::Expression const> Functor::inst(util::sref<SymbolTable const> st)
{
    _checkInst(st);
    return std::move(_result_or_nul_if_not_inst);
}

void Functor::_checkInst(util::sref<SymbolTable const> st)
{
    if (_result_or_nul_if_not_inst.nul()) {
        _result_or_nul_if_not_inst
              = st->queryVar(pos, name).call(pos, instForTypes(_args, st), instForArgs(_args, st));
    }
}

util::sref<inst::Type const> FuncReference::type(util::sref<SymbolTable const> st)
{
    return _mkType(st);
}

util::sptr<inst::Expression const> FuncReference::inst(util::sref<SymbolTable const> st)
{
    util::sref<FuncReferenceType const> ft = _mkType(st);
    return util::mkptr(new inst::FuncReference(ft->size, ft->makeCallArgs(), ft));
}

util::sref<FuncReferenceType const> FuncReference::_mkType(util::sref<SymbolTable const> st)
{
    std::map<std::string, Variable const> ext_vars(_func->bindExternalVars(pos, st));
    auto find_result = _type_cache.find(ext_vars);
    if (_type_cache.end() != find_result) {
        return util::mkref(find_result->second);
    }
    return util::mkref(_type_cache.insert(std::make_pair(ext_vars, FuncReferenceType(
                                                                        pos
                                                                      , _func
                                                                      , st->level
                                                                      , ext_vars))).first->second);
}

util::sref<inst::Type const> BinaryOp::type(util::sref<SymbolTable const> st)
{
    Operation const* o = st->queryBinary(pos, op, lhs->type(st), rhs->type(st));
    return o->ret_type;
}

util::sptr<inst::Expression const> BinaryOp::inst(util::sref<SymbolTable const> st)
{
    Operation const* o = st->queryBinary(pos, op, lhs->type(st), rhs->type(st));
    return util::mkptr(new inst::BinaryOp(lhs->inst(st), o->op_img, o->ret_type, rhs->inst(st)));
}

util::sref<inst::Type const> PreUnaryOp::type(util::sref<SymbolTable const> st)
{
    Operation const* o = st->queryPreUnary(pos, op, rhs->type(st));
    return o->ret_type;
}

util::sptr<inst::Expression const> PreUnaryOp::inst(util::sref<SymbolTable const> st)
{
    Operation const* o = st->queryPreUnary(pos, op, rhs->type(st));
    return util::mkptr(new inst::PreUnaryOp(o->op_img, o->ret_type, rhs->inst(st)));
}

util::sref<inst::Type const> Conjunction::type(util::sref<SymbolTable const>)
{
    return inst::Type::BIT_BOOL;
}

util::sptr<inst::Expression const> Conjunction::inst(util::sref<SymbolTable const> st)
{
    return util::mkptr(new inst::Conjunction(pos, lhs->inst(st), rhs->inst(st)));
}

util::sref<inst::Type const> Disjunction::type(util::sref<SymbolTable const>)
{
    return inst::Type::BIT_BOOL;
}

util::sptr<inst::Expression const> Disjunction::inst(util::sref<SymbolTable const> st)
{
    return util::mkptr(new inst::Disjunction(pos, lhs->inst(st), rhs->inst(st)));
}

util::sref<inst::Type const> Negation::type(util::sref<SymbolTable const>)
{
    return inst::Type::BIT_BOOL;
}

util::sptr<inst::Expression const> Negation::inst(util::sref<SymbolTable const> st)
{
    return util::mkptr(new inst::Negation(pos, rhs->inst(st)));
}
