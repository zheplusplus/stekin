#include <algorithm>

#include "expr-nodes.h"
#include "function.h"
#include "block.h"
#include "func-inst-draft.h"
#include "symbol-table.h"
#include "operation.h"
#include "variable.h"
#include "../instance/expr-nodes.h"

using namespace proto;

util::sref<Type const> BoolLiteral::type(util::sref<SymbolTable const>) const
{
    return Type::BIT_BOOL;
}

util::sptr<inst::Expression const> BoolLiteral::inst(util::sref<SymbolTable const>) const
{
    return util::mkptr(new inst::BoolLiteral(value));
}

util::sref<Type const> IntLiteral::type(util::sref<SymbolTable const>) const
{
    return Type::BIT_INT;
}

util::sptr<inst::Expression const> IntLiteral::inst(util::sref<SymbolTable const>) const
{
    return util::mkptr(new inst::IntLiteral(value.get_si()));
}

util::sref<Type const> FloatLiteral::type(util::sref<SymbolTable const>) const
{
    return Type::BIT_FLOAT;
}

util::sptr<inst::Expression const> FloatLiteral::inst(util::sref<SymbolTable const>) const
{
    return util::mkptr(new inst::FloatLiteral(value.get_d()));
}

util::sref<Type const> Reference::type(util::sref<SymbolTable const> st) const
{
    return st->queryVar(pos, name).type;
}

util::sptr<inst::Expression const> Reference::inst(util::sref<SymbolTable const> st) const
{
    Variable var(st->queryVar(pos, name));
    return util::mkptr(new inst::Reference(var.type->exportedName()
                                         , inst::Address(var.level, var.stack_offset)));
}

static std::vector<util::sptr<inst::Expression const>> instForArgs(
                                            std::vector<util::sptr<Expression const>> const& args
                                          , util::sref<SymbolTable const> st)
{
    std::vector<util::sptr<inst::Expression const>> arg_instances;
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& arg)
                  {
                      arg_instances.push_back(arg->inst(st));
                  });
    return std::move(arg_instances);
}

static std::vector<util::sref<Type const>> instForTypes(
                                            std::vector<util::sptr<Expression const>> const& args
                                          , util::sref<SymbolTable const> st)
{
    std::vector<util::sref<Type const>> arg_types;
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& arg)
                  {
                      arg_types.push_back(arg->type(st));
                  });
    return arg_types;
}

util::sref<Type const> Call::type(util::sref<SymbolTable const> st) const
{
    return _func->inst(pos, st, instForTypes(_args, st))->getReturnType();
}

util::sptr<inst::Expression const> Call::inst(util::sref<SymbolTable const> st) const
{
    util::sref<FuncInstDraft> draft(_func->inst(pos, st, instForTypes(_args, st)));
    return util::mkptr(new inst::Call(draft->sn, instForArgs(_args, st)));
}

util::sref<Type const> Functor::type(util::sref<SymbolTable const> st) const
{
    return _mkDraft(st)->getReturnType();
}

util::sptr<inst::Expression const> Functor::inst(util::sref<SymbolTable const> st) const
{
    util::sref<FuncInstDraft> draft(_mkDraft(st));
    return util::mkptr(new inst::Call(draft->sn, instForArgs(_args, st)));
}

util::sref<FuncInstDraft> Functor::_mkDraft(util::sref<SymbolTable const> st) const
{
    return st->queryVar(pos, name).call(pos, instForTypes(_args, st));
}

util::sref<Type const> FuncReference::type(util::sref<SymbolTable const> st) const
{
    return _func->refType(pos, st);
}

util::sptr<inst::Expression const> FuncReference::inst(util::sref<SymbolTable const> st) const
{
    util::sref<FuncReferenceType const> ft(_func->refType(pos, st));
    return util::mkptr(new inst::FuncReference(ft->size, ft->makeCallArgs()));
}

util::sref<Type const> BinaryOp::type(util::sref<SymbolTable const> st) const
{
    return st->queryBinary(pos, op, lhs->type(st), rhs->type(st))->ret_type;
}

util::sptr<inst::Expression const> BinaryOp::inst(util::sref<SymbolTable const> st) const
{
    util::sref<Operation const> o(st->queryBinary(pos, op, lhs->type(st), rhs->type(st)));
    return util::mkptr(new inst::BinaryOp(lhs->inst(st), o->op_img, rhs->inst(st)));
}

util::sref<Type const> PreUnaryOp::type(util::sref<SymbolTable const> st) const
{
    return st->queryPreUnary(pos, op, rhs->type(st))->ret_type;
}

util::sptr<inst::Expression const> PreUnaryOp::inst(util::sref<SymbolTable const> st) const
{
    util::sref<Operation const> o(st->queryPreUnary(pos, op, rhs->type(st)));
    return util::mkptr(new inst::PreUnaryOp(o->op_img, rhs->inst(st)));
}

util::sref<Type const> Conjunction::type(util::sref<SymbolTable const>) const
{
    return Type::BIT_BOOL;
}

util::sptr<inst::Expression const> Conjunction::inst(util::sref<SymbolTable const> st) const
{
    lhs->type(st)->checkCondType(pos);
    rhs->type(st)->checkCondType(pos);
    return util::mkptr(new inst::Conjunction(lhs->inst(st), rhs->inst(st)));
}

util::sref<Type const> Disjunction::type(util::sref<SymbolTable const>) const
{
    return Type::BIT_BOOL;
}

util::sptr<inst::Expression const> Disjunction::inst(util::sref<SymbolTable const> st) const
{
    lhs->type(st)->checkCondType(pos);
    rhs->type(st)->checkCondType(pos);
    return util::mkptr(new inst::Disjunction(lhs->inst(st), rhs->inst(st)));
}

util::sref<Type const> Negation::type(util::sref<SymbolTable const>) const
{
    return Type::BIT_BOOL;
}

util::sptr<inst::Expression const> Negation::inst(util::sref<SymbolTable const> st) const
{
    rhs->type(st)->checkCondType(pos);
    return util::mkptr(new inst::Negation(rhs->inst(st)));
}
