#include <algorithm>

#include "expr-nodes.h"
#include "function.h"
#include "func-inst-draft.h"
#include "symbol-table.h"
#include "operation.h"
#include "variable.h"
#include "../instance/expr-nodes.h"

using namespace proto;

util::sref<Type const> BoolLiteral::type(util::sref<SymbolTable const>)
{
    return Type::BIT_BOOL;
}

util::sptr<inst::Expression const> BoolLiteral::inst(util::sref<SymbolTable const>)
{
    return util::mkptr(new inst::BoolLiteral(value));
}

util::sref<Type const> IntLiteral::type(util::sref<SymbolTable const>)
{
    return Type::BIT_INT;
}

util::sptr<inst::Expression const> IntLiteral::inst(util::sref<SymbolTable const>)
{
    return util::mkptr(new inst::IntLiteral(value.get_si()));
}

util::sref<Type const> FloatLiteral::type(util::sref<SymbolTable const>)
{
    return Type::BIT_FLOAT;
}

util::sptr<inst::Expression const> FloatLiteral::inst(util::sref<SymbolTable const>)
{
    return util::mkptr(new inst::FloatLiteral(value.get_d()));
}

util::sref<Type const> Reference::type(util::sref<SymbolTable const> st)
{
    return st->queryVar(pos, name).type;
}

util::sptr<inst::Expression const> Reference::inst(util::sref<SymbolTable const> st)
{
    Variable var(st->queryVar(pos, name));
    return util::mkptr(new inst::Reference(var.type->exportedName(), var.level, var.stack_offset));
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

static std::vector<util::sref<Type const>> instForTypes(
                                            std::vector<util::sptr<Expression>> const& args
                                          , util::sref<SymbolTable const> st)
{
    std::vector<util::sref<Type const>> arg_types;
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression> const& arg)
                  {
                      arg_types.push_back(arg->type(st));
                  });
    return arg_types;
}

util::sref<Type const> Call::type(util::sref<SymbolTable const> st)
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
        _result_or_nul_if_not_inst.reset(new inst::Call(draft.id(), instForArgs(_args, st)));
    }
}

util::sref<Type const> Functor::type(util::sref<SymbolTable const> st)
{
    return _mkDraft(st)->getReturnType();
}

util::sptr<inst::Expression const> Functor::inst(util::sref<SymbolTable const> st)
{
    util::sref<FuncInstDraft> draft(_mkDraft(st));
    return util::mkptr(new inst::Call(draft.id(), instForArgs(_args, st)));
}

util::sref<FuncInstDraft> Functor::_mkDraft(util::sref<SymbolTable const> st) const
{
    return st->queryVar(pos, name).call(pos, instForTypes(_args, st));
}

util::sref<Type const> FuncReference::type(util::sref<SymbolTable const> st)
{
    return _mkType(st);
}

util::sptr<inst::Expression const> FuncReference::inst(util::sref<SymbolTable const> st)
{
    util::sref<FuncReferenceType const> ft(_mkType(st));
    return util::mkptr(new inst::FuncReference(ft->size, ft->makeCallArgs()));
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

util::sref<Type const> BinaryOp::type(util::sref<SymbolTable const> st)
{
    Operation const* o = st->queryBinary(pos, op, lhs->type(st), rhs->type(st));
    return o->ret_type;
}

util::sptr<inst::Expression const> BinaryOp::inst(util::sref<SymbolTable const> st)
{
    Operation const* o = st->queryBinary(pos, op, lhs->type(st), rhs->type(st));
    return util::mkptr(new inst::BinaryOp(lhs->inst(st), o->op_img, rhs->inst(st)));
}

util::sref<Type const> PreUnaryOp::type(util::sref<SymbolTable const> st)
{
    Operation const* o = st->queryPreUnary(pos, op, rhs->type(st));
    return o->ret_type;
}

util::sptr<inst::Expression const> PreUnaryOp::inst(util::sref<SymbolTable const> st)
{
    Operation const* o = st->queryPreUnary(pos, op, rhs->type(st));
    return util::mkptr(new inst::PreUnaryOp(o->op_img, rhs->inst(st)));
}

util::sref<Type const> Conjunction::type(util::sref<SymbolTable const>)
{
    return Type::BIT_BOOL;
}

util::sptr<inst::Expression const> Conjunction::inst(util::sref<SymbolTable const> st)
{
    lhs->type(st)->checkCondType(pos);
    rhs->type(st)->checkCondType(pos);
    return util::mkptr(new inst::Conjunction(lhs->inst(st), rhs->inst(st)));
}

util::sref<Type const> Disjunction::type(util::sref<SymbolTable const>)
{
    return Type::BIT_BOOL;
}

util::sptr<inst::Expression const> Disjunction::inst(util::sref<SymbolTable const> st)
{
    lhs->type(st)->checkCondType(pos);
    rhs->type(st)->checkCondType(pos);
    return util::mkptr(new inst::Disjunction(lhs->inst(st), rhs->inst(st)));
}

util::sref<Type const> Negation::type(util::sref<SymbolTable const>)
{
    return Type::BIT_BOOL;
}

util::sptr<inst::Expression const> Negation::inst(util::sref<SymbolTable const> st)
{
    rhs->type(st)->checkCondType(pos);
    return util::mkptr(new inst::Negation(rhs->inst(st)));
}
