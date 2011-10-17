#include <algorithm>

#include <instance/expr-nodes.h>
#include <instance/built-in.h>
#include <report/errors.h>

#include "expr-nodes.h"
#include "function.h"
#include "block.h"
#include "func-inst-draft.h"
#include "symbol-table.h"
#include "operation.h"
#include "variable.h"
#include "list-types.h"
#include "list-context.h"

using namespace proto;

util::sref<Type const> BoolLiteral::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return Type::BIT_BOOL;
}

util::sptr<inst::Expression const> BoolLiteral::inst(util::sref<SymbolTable const>
                                                   , misc::trace&) const
{
    return util::mkptr(new inst::BoolLiteral(value));
}

util::sref<Type const> IntLiteral::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return Type::BIT_INT;
}

util::sptr<inst::Expression const> IntLiteral::inst(util::sref<SymbolTable const>
                                                  , misc::trace&) const
{
    return util::mkptr(new inst::IntLiteral(value.get_si()));
}

util::sref<Type const> FloatLiteral::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return Type::BIT_FLOAT;
}

util::sptr<inst::Expression const> FloatLiteral::inst(util::sref<SymbolTable const>
                                                    , misc::trace&) const
{
    return util::mkptr(new inst::FloatLiteral(value.get_d()));
}

static std::vector<util::sptr<inst::Expression const>> instForExprs(
                                            std::vector<util::sptr<Expression const>> const& exprs
                                          , util::sref<SymbolTable const> st
                                          , misc::trace& trace)
{
    std::vector<util::sptr<inst::Expression const>> expr_instances;
    std::for_each(exprs.begin()
                , exprs.end()
                , [&](util::sptr<Expression const> const& expr)
                  {
                      expr_instances.push_back(expr->inst(st, trace));
                  });
    return std::move(expr_instances);
}

static std::vector<util::sptr<inst::Expression const>> instForExprsAsPipe(
                                            std::vector<util::sptr<Expression const>> const& exprs
                                          , util::sref<SymbolTable const> st
                                          , util::sref<ListContext const> lc
                                          , misc::trace& trace)
{
    std::vector<util::sptr<inst::Expression const>> expr_instances;
    std::for_each(exprs.begin()
                , exprs.end()
                , [&](util::sptr<Expression const> const& expr)
                  {
                      expr_instances.push_back(expr->instAsPipe(st, lc, trace));
                  });
    return std::move(expr_instances);
}

static std::vector<util::sref<Type const>> instForTypes(
                                            std::vector<util::sptr<Expression const>> const& args
                                          , util::sref<SymbolTable const> st
                                          , misc::trace& trace)
{
    std::vector<util::sref<Type const>> arg_types;
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& arg)
                  {
                      arg_types.push_back(arg->type(st, trace));
                  });
    return arg_types;
}

static std::vector<util::sref<Type const>> instForTypesAsPipe(
                                            std::vector<util::sptr<Expression const>> const& args
                                          , util::sref<SymbolTable const> st
                                          , util::sref<ListContext const> lc
                                          , misc::trace& trace)
{
    std::vector<util::sref<Type const>> arg_types;
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& arg)
                  {
                      arg_types.push_back(arg->typeAsPipe(st, lc, trace));
                  });
    return arg_types;
}

static void checkMemberTypes(misc::position const& pos
                           , std::vector<util::sref<Type const>> const& member_types)
{
    util::sref<Type const> first_type = member_types[0];
    for (auto i = ++member_types.begin(); member_types.end() != i; ++i) {
        if (*i != first_type) {
            error::listMemberTypesNotSame(pos);
            return;
        }
    }
}

util::sref<Type const> ListLiteral::type(util::sref<SymbolTable const> st, misc::trace& trace) const
{
    if (0 == value.size()) {
        return ListType::getEmptyListType();
    }
    return ListType::getListType(_memberType(st, trace));
}

util::sptr<inst::Expression const> ListLiteral::inst(util::sref<SymbolTable const> st
                                                   , misc::trace& trace) const
{
    if (0 == value.size()) {
        return util::mkptr(new inst::EmptyListLiteral);
    }
    return util::mkptr(new inst::ListLiteral(_memberType(st, trace)->makeInstType()
                                           , instForExprs(value, st, trace)));
}

util::sref<Type const> ListLiteral::typeAsPipe(util::sref<SymbolTable const> st
                                             , util::sref<ListContext const> lc
                                             , misc::trace& trace) const
{
    if (0 == value.size()) {
        return ListType::getEmptyListType();
    }
    return ListType::getListType(_memberTypeAsPipe(st, lc, trace));
}

util::sptr<inst::Expression const> ListLiteral::instAsPipe(util::sref<SymbolTable const> st
                                                         , util::sref<ListContext const> lc
                                                         , misc::trace& trace) const
{
    if (0 == value.size()) {
        return util::mkptr(new inst::EmptyListLiteral);
    }
    return util::mkptr(new inst::ListLiteral(_memberTypeAsPipe(st, lc, trace)->makeInstType()
                                           , instForExprsAsPipe(value, st, lc, trace)));
}

util::sref<Type const> ListLiteral::_memberType(util::sref<SymbolTable const> st
                                              , misc::trace& trace) const
{
    std::vector<util::sref<Type const>> member_types(instForTypes(value, st, trace));
    checkMemberTypes(pos, member_types);
    return member_types[0];
}

util::sref<Type const> ListLiteral::_memberTypeAsPipe(util::sref<SymbolTable const> st
                                                    , util::sref<ListContext const> lc
                                                    , misc::trace& trace) const
{
    std::vector<util::sref<Type const>> member_types(instForTypesAsPipe(value, st, lc, trace));
    checkMemberTypes(pos, member_types);
    return member_types[0];
}

util::sref<Type const> ListElement::type(util::sref<SymbolTable const>, misc::trace&) const
{
    error::pipeReferenceNotInListContext(pos);
    return Type::BIT_INT;
}

util::sptr<inst::Expression const> ListElement::inst(util::sref<SymbolTable const>
                                                   , misc::trace&) const
{
    error::pipeReferenceNotInListContext(pos);
    return util::mkptr(new inst::ListIndex);
}

util::sref<Type const> ListElement::typeAsPipe(util::sref<SymbolTable const>
                                             , util::sref<ListContext const> lc
                                             , misc::trace&) const
{
    return lc->member_type;
}

util::sptr<inst::Expression const> ListElement::instAsPipe(util::sref<SymbolTable const>
                                                         , util::sref<ListContext const>
                                                         , misc::trace&) const
{
    return util::mkptr(new inst::ListElement);
}

util::sref<Type const> ListIndex::type(util::sref<SymbolTable const>, misc::trace&) const
{
    error::pipeReferenceNotInListContext(pos);
    return Type::BIT_INT;
}

util::sptr<inst::Expression const> ListIndex::inst(util::sref<SymbolTable const>
                                                 , misc::trace&) const
{
    error::pipeReferenceNotInListContext(pos);
    return util::mkptr(new inst::ListIndex);
}

util::sref<Type const> ListIndex::typeAsPipe(util::sref<SymbolTable const>
                                           , util::sref<ListContext const>
                                           , misc::trace&) const
{
    return Type::BIT_INT;
}

util::sptr<inst::Expression const> ListIndex::instAsPipe(util::sref<SymbolTable const>
                                                       , util::sref<ListContext const>
                                                       , misc::trace&) const
{
    return util::mkptr(new inst::ListIndex);
}

util::sref<Type const> Reference::type(util::sref<SymbolTable const> st, misc::trace&) const
{
    return st->queryVar(pos, name).type;
}

util::sptr<inst::Expression const> Reference::inst(util::sref<SymbolTable const> st
                                                 , misc::trace&) const
{
    Variable var(st->queryVar(pos, name));
    return util::mkptr(new inst::Reference(var.type->makeInstType()
                                         , inst::Address(var.level, var.stack_offset)));
}

util::sref<Type const> Call::type(util::sref<SymbolTable const> st, misc::trace& trace) const
{
    trace.add(pos);
    return _func->inst(st, instForTypes(_args, st, trace), trace)->getReturnType();
}

util::sptr<inst::Expression const> Call::inst(util::sref<SymbolTable const> st
                                            , misc::trace& trace) const
{
    trace.add(pos);
    util::sref<FuncInstDraft> draft(_func->inst(st, instForTypes(_args, st, trace), trace));
    return util::mkptr(new inst::Call(draft->sn, instForExprs(_args, st, trace)));
}

util::sref<Type const> Call::typeAsPipe(util::sref<SymbolTable const> st
                                      , util::sref<ListContext const> lc
                                      , misc::trace& trace) const
{
    trace.add(pos);
    return _func->inst(st, instForTypesAsPipe(_args, st, lc, trace), trace)->getReturnType();
}

util::sptr<inst::Expression const> Call::instAsPipe(util::sref<SymbolTable const> st
                                                  , util::sref<ListContext const> lc
                                                  , misc::trace& trace) const
{
    trace.add(pos);
    util::sref<FuncInstDraft> draft(
                                _func->inst(st, instForTypesAsPipe(_args, st, lc, trace), trace));
    return util::mkptr(new inst::Call(draft->sn, instForExprsAsPipe(_args, st, lc, trace)));
}

util::sref<Type const> MemberCall::type(util::sref<SymbolTable const> st, misc::trace& trace) const
{
    trace.add(pos);
    return object->type(st, trace)
                 ->memberCallType(member_call, instForTypes(args, st, trace), trace);
}

util::sptr<inst::Expression const> MemberCall::inst(util::sref<SymbolTable const> st
                                                  , misc::trace& trace) const
{
    trace.add(pos);
    return util::mkptr(new inst::MemberCall(object->inst(st, trace)
                                          , member_call
                                          , instForExprs(args, st, trace)));
}

util::sref<Type const> MemberCall::typeAsPipe(util::sref<SymbolTable const> st
                                            , util::sref<ListContext const> lc
                                            , misc::trace& trace) const
{
    trace.add(pos);
    return object->typeAsPipe(st, lc, trace)
                 ->memberCallType(member_call, instForTypesAsPipe(args, st, lc, trace), trace);
}

util::sptr<inst::Expression const> MemberCall::instAsPipe(util::sref<SymbolTable const> st
                                                        , util::sref<ListContext const> lc
                                                        , misc::trace& trace) const
{
    trace.add(pos);
    return util::mkptr(new inst::MemberCall(object->instAsPipe(st, lc, trace)
                                          , member_call
                                          , instForExprsAsPipe(args, st, lc, trace)));
}

util::sref<Type const> Functor::type(util::sref<SymbolTable const> st, misc::trace& trace) const
{
    return _mkDraft(st, trace)->getReturnType();
}

util::sptr<inst::Expression const> Functor::inst(util::sref<SymbolTable const> st
                                               , misc::trace& trace) const
{
    util::sref<FuncInstDraft> draft(_mkDraft(st, trace));
    return util::mkptr(new inst::Call(draft->sn, instForExprs(_args, st, trace)));
}

util::sref<Type const> Functor::typeAsPipe(util::sref<SymbolTable const> st
                                         , util::sref<ListContext const> lc
                                         , misc::trace& trace) const
{
    return _mkDraftAsPipe(st, lc, trace)->getReturnType();
}

util::sptr<inst::Expression const> Functor::instAsPipe(util::sref<SymbolTable const> st
                                                     , util::sref<ListContext const> lc
                                                     , misc::trace& trace) const
{
    util::sref<FuncInstDraft> draft(_mkDraftAsPipe(st, lc, trace));
    return util::mkptr(new inst::Call(draft->sn, instForExprsAsPipe(_args, st, lc, trace)));
}

util::sref<FuncInstDraft> Functor::_mkDraft(util::sref<SymbolTable const> st
                                          , misc::trace& trace) const
{
    trace.add(pos);
    return st->queryVar(pos, name).call(instForTypes(_args, st, trace), trace);
}

util::sref<FuncInstDraft> Functor::_mkDraftAsPipe(util::sref<SymbolTable const> st
                                                , util::sref<ListContext const> lc
                                                , misc::trace& trace) const
{
    trace.add(pos);
    return st->queryVar(pos, name).call(instForTypesAsPipe(_args, st, lc, trace), trace);
}

util::sref<Type const> FuncReference::type(util::sref<SymbolTable const> st, misc::trace&) const
{
    return _func->refType(pos, st);
}

util::sptr<inst::Expression const> FuncReference::inst(util::sref<SymbolTable const> st
                                                     , misc::trace&) const
{
    util::sref<FuncReferenceType const> ft(_func->refType(pos, st));
    return util::mkptr(new inst::FuncReference(ft->size, ft->makeCallArgs()));
}

static util::sref<Type const> listAppendType(misc::position const& pos
                                           , util::sref<Type const> lhs_type
                                           , util::sref<Type const> rhs_type)
{
    if (!(ListType::isListType(lhs_type) && ListType::isListType(rhs_type))) {
        error::binaryOpNotAvai(pos, "++", lhs_type->name(), rhs_type->name());
        return Type::BAD_TYPE;
    }
    util::sref<Type const> result(ListType::commonListTypeOrNulIfImcompatible(lhs_type, rhs_type));
    if (result.nul()) {
        error::binaryOpNotAvai(pos, "++", lhs_type->name(), rhs_type->name());
        return Type::BAD_TYPE;
    }
    return result;
}

util::sref<Type const> ListAppend::type(util::sref<SymbolTable const> st, misc::trace& trace) const
{
    return listAppendType(pos, lhs->type(st, trace), rhs->type(st, trace));
}

util::sptr<inst::Expression const> ListAppend::inst(util::sref<SymbolTable const> st
                                                  , misc::trace& trace) const
{
    return util::mkptr(new inst::ListAppend(lhs->inst(st, trace), rhs->inst(st, trace)));
}

util::sref<Type const> ListAppend::typeAsPipe(util::sref<SymbolTable const> st
                                            , util::sref<ListContext const> lc
                                            , misc::trace& trace) const
{
    return listAppendType(pos, lhs->typeAsPipe(st, lc, trace), rhs->typeAsPipe(st, lc, trace));
}

util::sptr<inst::Expression const> ListAppend::instAsPipe(util::sref<SymbolTable const> st
                                                        , util::sref<ListContext const> lc
                                                        , misc::trace& trace) const
{
    return util::mkptr(new inst::ListAppend(lhs->instAsPipe(st, lc, trace)
                                          , rhs->instAsPipe(st, lc, trace)));
}

util::sref<Type const> BinaryOp::type(util::sref<SymbolTable const> st, misc::trace& trace) const
{
    return st->queryBinary(pos, op, lhs->type(st, trace), rhs->type(st, trace))->ret_type;
}

util::sptr<inst::Expression const> BinaryOp::inst(util::sref<SymbolTable const> st
                                                , misc::trace& trace) const
{
    util::sref<Operation const> o(
            st->queryBinary(pos, op, lhs->type(st, trace), rhs->type(st, trace)));
    return util::mkptr(new inst::BinaryOp(lhs->inst(st, trace), o->op_img, rhs->inst(st, trace)));
}

util::sref<Type const> BinaryOp::typeAsPipe(util::sref<SymbolTable const> st
                                          , util::sref<ListContext const> lc
                                          , misc::trace& trace) const
{
    return st->queryBinary(pos, op, lhs->typeAsPipe(st, lc, trace), rhs->typeAsPipe(st, lc, trace))
             ->ret_type;
}

util::sptr<inst::Expression const> BinaryOp::instAsPipe(util::sref<SymbolTable const> st
                                                      , util::sref<ListContext const> lc
                                                      , misc::trace& trace) const
{
    util::sref<Operation const> o(st->queryBinary(
                    pos, op, lhs->typeAsPipe(st, lc, trace), rhs->typeAsPipe(st, lc, trace)));
    return util::mkptr(new inst::BinaryOp(lhs->instAsPipe(st, lc, trace)
                                        , o->op_img
                                        , rhs->instAsPipe(st, lc, trace)));
}

util::sref<Type const> PreUnaryOp::type(util::sref<SymbolTable const> st, misc::trace& trace) const
{
    return st->queryPreUnary(pos, op, rhs->type(st, trace))->ret_type;
}

util::sptr<inst::Expression const> PreUnaryOp::inst(util::sref<SymbolTable const> st
                                                  , misc::trace& trace) const
{
    util::sref<Operation const> o(st->queryPreUnary(pos, op, rhs->type(st, trace)));
    return util::mkptr(new inst::PreUnaryOp(o->op_img, rhs->inst(st, trace)));
}

util::sref<Type const> PreUnaryOp::typeAsPipe(util::sref<SymbolTable const> st
                                            , util::sref<ListContext const> lc
                                            , misc::trace& trace) const
{
    return st->queryPreUnary(pos, op, rhs->typeAsPipe(st, lc, trace))->ret_type;
}

util::sptr<inst::Expression const> PreUnaryOp::instAsPipe(util::sref<SymbolTable const> st
                                                        , util::sref<ListContext const> lc
                                                        , misc::trace& trace) const
{
    util::sref<Operation const> o(st->queryPreUnary(pos, op, rhs->typeAsPipe(st, lc, trace)));
    return util::mkptr(new inst::PreUnaryOp(o->op_img, rhs->instAsPipe(st, lc, trace)));
}

util::sref<Type const> Conjunction::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return Type::BIT_BOOL;
}

util::sptr<inst::Expression const> Conjunction::inst(util::sref<SymbolTable const> st
                                                   , misc::trace& trace) const
{
    lhs->type(st, trace)->checkCondType(pos);
    rhs->type(st, trace)->checkCondType(pos);
    return util::mkptr(new inst::Conjunction(lhs->inst(st, trace), rhs->inst(st, trace)));
}

util::sptr<inst::Expression const> Conjunction::instAsPipe(util::sref<SymbolTable const> st
                                                         , util::sref<ListContext const> lc
                                                         , misc::trace& trace) const
{
    lhs->typeAsPipe(st, lc, trace)->checkCondType(pos);
    rhs->typeAsPipe(st, lc, trace)->checkCondType(pos);
    return util::mkptr(new inst::Conjunction(lhs->instAsPipe(st, lc, trace)
                                           , rhs->instAsPipe(st, lc, trace)));
}

util::sref<Type const> Disjunction::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return Type::BIT_BOOL;
}

util::sptr<inst::Expression const> Disjunction::inst(util::sref<SymbolTable const> st
                                                   , misc::trace& trace) const
{
    lhs->type(st, trace)->checkCondType(pos);
    rhs->type(st, trace)->checkCondType(pos);
    return util::mkptr(new inst::Disjunction(lhs->inst(st, trace), rhs->inst(st, trace)));
}

util::sptr<inst::Expression const> Disjunction::instAsPipe(util::sref<SymbolTable const> st
                                                         , util::sref<ListContext const> lc
                                                         , misc::trace& trace) const
{
    lhs->typeAsPipe(st, lc, trace)->checkCondType(pos);
    rhs->typeAsPipe(st, lc, trace)->checkCondType(pos);
    return util::mkptr(new inst::Disjunction(lhs->instAsPipe(st, lc, trace)
                                           , rhs->instAsPipe(st, lc, trace)));
}

util::sref<Type const> Negation::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return Type::BIT_BOOL;
}

util::sptr<inst::Expression const> Negation::inst(util::sref<SymbolTable const> st
                                                , misc::trace& trace) const
{
    rhs->type(st, trace)->checkCondType(pos);
    return util::mkptr(new inst::Negation(rhs->inst(st, trace)));
}

util::sptr<inst::Expression const> Negation::instAsPipe(util::sref<SymbolTable const> st
                                                      , util::sref<ListContext const> lc
                                                      , misc::trace& trace) const
{
    rhs->typeAsPipe(st, lc, trace)->checkCondType(pos);
    return util::mkptr(new inst::Negation(rhs->instAsPipe(st, lc, trace)));
}
