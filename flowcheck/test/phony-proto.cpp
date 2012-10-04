#include <algorithm>
#include <vector>
#include <map>

#include <proto/stmt-nodes.h>
#include <proto/expr-nodes.h>
#include <proto/list-pipe.h>
#include <proto/function.h>
#include <proto/built-in.h>
#include <instance/node-base.h>
#include <instance/list-pipe.h>
#include <util/string.h>

#include "test-common.h"

using namespace test;
using namespace proto;

static std::list<util::sptr<Function>> func_entities;
static util::sptr<inst::Expression const> NUL_INST_EXPR(nullptr);
static util::sptr<inst::Statement const> NUL_INST_STMT(nullptr);
static util::sref<Type const> NUL_TYPE(nullptr);

void Block::addTo(util::sref<FuncInstDraft>) {}
void Block::mediateInst(util::sref<FuncInstDraft>, misc::trace&) {}

void Block::addStmt(util::sptr<Statement> stmt)
{
    _stmts.push_back(std::move(stmt));
}

util::sptr<inst::Statement const> Block::inst(util::sref<FuncInstDraft>, misc::trace&)
{
    DataTree::actualOne()(SCOPE_BEGIN);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement> const& stmt)
                  {
                      stmt->inst(nul_draft, nultrace);
                  });
    DataTree::actualOne()(SCOPE_END);
    return std::move(NUL_INST_STMT);
}

std::vector<util::sptr<inst::Function const>> Block::deliverFuncs()
{
    return std::vector<util::sptr<inst::Function const>>();
}

util::sref<Function> Block::declare(misc::position const& pos
                                  , std::string const& name
                                  , std::vector<std::string> const& param_names
                                  , bool hint_return_void)
{
    DataTree::actualOne()(pos, FUNC_DECL, name, param_names.size(), hint_return_void);
    std::for_each(param_names.begin()
                , param_names.end()
                , [&](std::string const& param)
                  {
                      DataTree::actualOne()(pos, PARAMETER, param);
                  });
    func_entities.push_back(util::mkptr(new Function(pos, name, param_names, hint_return_void)));
    return *func_entities.back();
}

void DirectInst::addTo(util::sref<FuncInstDraft>) {}
void DirectInst::mediateInst(util::sref<FuncInstDraft>, misc::trace&) {}

util::sptr<inst::Statement const> DirectInst::inst(util::sref<FuncInstDraft>, misc::trace&)
{
    return _inst(nul_draft, nultrace);
}

std::vector<util::sptr<inst::Function const>> DirectInst::deliverFuncs()
{
    return std::vector<util::sptr<inst::Function const>>();
}

util::sptr<inst::Statement const> Return::_inst(util::sref<FuncInstDraft>, misc::trace&) const
{
    DataTree::actualOne()(pos, RETURN);
    ret_val->inst(nul_st, nultrace);
    return std::move(NUL_INST_STMT);
}

util::sptr<inst::Statement const> ReturnNothing::_inst(util::sref<FuncInstDraft>
                                                     , misc::trace&) const
{
    DataTree::actualOne()(pos, RETURN_NOTHING);
    return std::move(NUL_INST_STMT);
}

util::sptr<inst::Statement const> VarDef::_inst(util::sref<FuncInstDraft>, misc::trace&) const
{
    DataTree::actualOne()(pos, VAR_DEF, name);
    init->inst(nul_st, nultrace);
    return std::move(NUL_INST_STMT);
}

void Branch::addTo(util::sref<FuncInstDraft>) {}
void Branch::mediateInst(util::sref<FuncInstDraft>, misc::trace&) {}

util::sptr<inst::Statement const> Branch::inst(util::sref<FuncInstDraft>, misc::trace&)
{
    DataTree::actualOne()(pos, BRANCH);
    predicate->inst(nul_st, nultrace);
    _consequence_stmt->inst(nul_draft, nultrace);
    _alternative_stmt->inst(nul_draft, nultrace);
    return std::move(NUL_INST_STMT);
}

std::vector<util::sptr<inst::Function const>> Branch::deliverFuncs()
{
    return std::vector<util::sptr<inst::Function const>>();
}

util::sptr<inst::Statement const> Arithmetics::_inst(util::sref<FuncInstDraft>, misc::trace&) const
{
    DataTree::actualOne()(pos, ARITHMETICS);
    expr->inst(nul_st, nultrace);
    return std::move(NUL_INST_STMT);
}

util::sptr<inst::Expression const> BoolLiteral::inst(util::sref<SymbolTable const>
                                                   , misc::trace&) const
{
    DataTree::actualOne()(pos, BOOLEAN, util::str(value));
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> IntLiteral::inst(util::sref<SymbolTable const>
                                                  , misc::trace&) const
{
    DataTree::actualOne()(pos, INTEGER, util::str(value));
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> FloatLiteral::inst(util::sref<SymbolTable const>
                                                    , misc::trace&) const
{
    DataTree::actualOne()(pos, FLOATING, util::str(value));
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> StringLiteral::inst(util::sref<SymbolTable const>
                                                     , misc::trace&) const
{
    DataTree::actualOne()(pos, STRING, value);
    return std::move(NUL_INST_EXPR);
}

static void instList(std::vector<util::sptr<Expression const>> const& list)
{
    std::for_each(list.begin()
                , list.end()
                , [&](util::sptr<Expression const> const& member)
                  {
                      member->inst(nul_st, nultrace);
                  });
}

util::sptr<inst::Expression const> ListLiteral::inst(util::sref<SymbolTable const>
                                                   , misc::trace&) const
{
    DataTree::actualOne()(pos, LIST, value.size());
    instList(value);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> ListElement::inst(util::sref<SymbolTable const>
                                                   , misc::trace&) const
{
    DataTree::actualOne()(pos, LIST_ELEMENT);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> ListIndex::inst(util::sref<SymbolTable const>
                                                 , misc::trace&) const
{
    DataTree::actualOne()(pos, LIST_INDEX);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> ListAppend::inst(util::sref<SymbolTable const>
                                                   , misc::trace&) const
{
    DataTree::actualOne()(pos, BINARY_OP, "++");
    lhs->inst(nul_st, nultrace);
    rhs->inst(nul_st, nultrace);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> Reference::inst(util::sref<SymbolTable const>
                                                 , misc::trace&) const
{
    DataTree::actualOne()(pos, REFERENCE, name);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> Call::inst(util::sref<SymbolTable const>
                                            , misc::trace&) const
{
    DataTree::actualOne()(pos, CALL, _func->name, _args.size(), false);
    instList(_args);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> MemberCall::inst(util::sref<SymbolTable const>
                                                  , misc::trace&) const
{
    DataTree::actualOne()(pos, BINARY_OP, ".");
    object->inst(nul_st, nultrace);
    DataTree::actualOne()(pos, CALL, member_call, args.size(), false);
    instList(args);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> FuncReference::inst(util::sref<SymbolTable const>
                                                     , misc::trace&) const
{
    DataTree::actualOne()(pos, FUNC_REFERENCE, _func->name, _func->param_names.size(), false);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> Functor::inst(util::sref<SymbolTable const>
                                               , misc::trace&) const
{
    DataTree::actualOne()(pos, FUNCTOR, name, _args.size(), false);
    std::for_each(_args.begin()
                , _args.end()
                , [&](util::sptr<Expression const> const& arg)
                  {
                      arg->inst(nul_st, nultrace);
                  });
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> BinaryOp::inst(util::sref<SymbolTable const>
                                                , misc::trace&) const
{
    DataTree::actualOne()(pos, BINARY_OP, op);
    lhs->inst(nul_st, nultrace);
    rhs->inst(nul_st, nultrace);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> PreUnaryOp::inst(util::sref<SymbolTable const>
                                                  , misc::trace&) const
{
    DataTree::actualOne()(pos, PRE_UNARY_OP, op);
    rhs->inst(nul_st, nultrace);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> Conjunction::inst(util::sref<SymbolTable const>
                                                   , misc::trace&) const
{
    DataTree::actualOne()(pos, BINARY_OP, "&&");
    lhs->inst(nul_st, nultrace);
    rhs->inst(nul_st, nultrace);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> Disjunction::inst(util::sref<SymbolTable const>
                                                   , misc::trace&) const
{
    DataTree::actualOne()(pos, BINARY_OP, "||");
    lhs->inst(nul_st, nultrace);
    rhs->inst(nul_st, nultrace);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> Negation::inst(util::sref<SymbolTable const>
                                                , misc::trace&) const
{
    DataTree::actualOne()(pos, PRE_UNARY_OP, "!");
    rhs->inst(nul_st, nultrace);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> WriterExpr::inst(util::sref<SymbolTable const>
                                                  , misc::trace&) const
{
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> ListPipeline::inst(util::sref<SymbolTable const>
                                                    , misc::trace&) const
{
    DataTree::actualOne()(pos, LIST_PIPELINE, pipeline.size());
    list->inst(nul_st, nultrace);
    std::for_each(pipeline.begin()
                , pipeline.end()
                , [&](util::sptr<PipeBase const> const& pipe)
                  {
                      pipe->inst(nul_st, util::sref<ListContext const>(nullptr), nultrace);
                  });
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::PipeBase const> PipeMap::inst(util::sref<SymbolTable const>
                                             , util::sref<ListContext const>
                                             , misc::trace&) const
{
    DataTree::actualOne()(PIPE_MAP);
    expr->inst(nul_st, nultrace);
    return util::sptr<inst::PipeBase const>(nullptr);
}

util::sptr<inst::PipeBase const> PipeFilter::inst(util::sref<SymbolTable const>
                                                , util::sref<ListContext const>
                                                , misc::trace&) const
{
    DataTree::actualOne()(PIPE_FILTER);
    expr->inst(nul_st, nultrace);
    return util::sptr<inst::PipeBase const>(nullptr);
}

util::sref<Block> Function::block()
{
    return util::mkref(_block);
}

void Function::setFreeVariables(std::vector<std::string> const&) {}

util::sref<Type const> BoolLiteral::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> IntLiteral::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> FloatLiteral::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> StringLiteral::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> ListLiteral::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> ListElement::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> ListIndex::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> ListAppend::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> Reference::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> Call::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> MemberCall::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> Functor::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> FuncReference::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> BinaryOp::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> PreUnaryOp::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> Conjunction::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> Disjunction::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> Negation::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> WriterExpr::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> ListPipeline::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> PipeMap::typeTransfer(util::sref<SymbolTable const>
                                           , util::sref<ListContext const>
                                           , misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<Type const> PipeFilter::typeTransfer(util::sref<SymbolTable const>
                                              , util::sref<ListContext const>
                                              , misc::trace&) const
{
    return NUL_TYPE;
}

void Type::checkCondType(misc::position const&) const {}

util::sptr<inst::Type const> FuncReferenceType::makeInstType() const
{
    return util::sptr<inst::Type const>(nullptr);
}

std::string FuncReferenceType::name() const
{
    return "";
}

util::sref<FuncInstDraft> Type::call(int
                                   , int
                                   , std::vector<util::sref<Type const>> const&
                                   , misc::trace&) const
{
    return nul_draft;
}

std::vector<int> Type::resEntries(int) const
{
    return std::vector<int>();
}

util::sref<Type const> Type::memberCallType(std::string const&
                                          , std::vector<util::sref<Type const>> const&
                                          , misc::trace&) const
{
    return NUL_TYPE;
}

util::sref<FuncInstDraft> FuncReferenceType::call(int
                                                , int
                                                , std::vector<util::sref<Type const>> const&
                                                , misc::trace&) const
{
    return nul_draft;
}

util::sref<Type const> Expression::typeAsPipe(util::sref<SymbolTable const> st
                                            , util::sref<ListContext const>
                                            , misc::trace& trace) const
{
    return type(st, trace);
}

util::sptr<inst::Expression const> Expression::instAsPipe(util::sref<SymbolTable const> st
                                                        , util::sref<ListContext const>
                                                        , misc::trace& trace) const
{
    return inst(st, trace);
}

util::sref<Type const> ListLiteral::typeAsPipe(util::sref<SymbolTable const> st
                                             , util::sref<ListContext const>
                                             , misc::trace& trace) const
{
    return type(st, trace);
}

util::sptr<inst::Expression const> ListLiteral::instAsPipe(util::sref<SymbolTable const> st
                                                         , util::sref<ListContext const>
                                                         , misc::trace& trace) const
{
    return inst(st, trace);
}

util::sref<Type const> ListElement::typeAsPipe(util::sref<SymbolTable const> st
                                             , util::sref<ListContext const>
                                             , misc::trace& trace) const
{
    return type(st, trace);
}

util::sptr<inst::Expression const> ListElement::instAsPipe(util::sref<SymbolTable const> st
                                                         , util::sref<ListContext const>
                                                         , misc::trace& trace) const
{
    return inst(st, trace);
}

util::sref<Type const> ListIndex::typeAsPipe(util::sref<SymbolTable const> st
                                           , util::sref<ListContext const>
                                           , misc::trace& trace) const
{
    return type(st, trace);
}

util::sptr<inst::Expression const> ListIndex::instAsPipe(util::sref<SymbolTable const> st
                                                       , util::sref<ListContext const>
                                                       , misc::trace& trace) const
{
    return inst(st, trace);
}

util::sref<Type const> Call::typeAsPipe(util::sref<SymbolTable const> st
                                      , util::sref<ListContext const>
                                      , misc::trace& trace) const
{
    return type(st, trace);
}

util::sptr<inst::Expression const> Call::instAsPipe(util::sref<SymbolTable const> st
                                                  , util::sref<ListContext const>
                                                  , misc::trace& trace) const
{
    return inst(st, trace);
}

util::sref<Type const> MemberCall::typeAsPipe(util::sref<SymbolTable const> st
                                            , util::sref<ListContext const>
                                            , misc::trace& trace) const
{
    return type(st, trace);
}

util::sptr<inst::Expression const> MemberCall::instAsPipe(util::sref<SymbolTable const> st
                                                        , util::sref<ListContext const>
                                                        , misc::trace& trace) const
{
    return inst(st, trace);
}

util::sref<Type const> Functor::typeAsPipe(util::sref<SymbolTable const> st
                                         , util::sref<ListContext const>
                                         , misc::trace& trace) const
{
    return type(st, trace);
}

util::sptr<inst::Expression const> Functor::instAsPipe(util::sref<SymbolTable const> st
                                                     , util::sref<ListContext const>
                                                     , misc::trace& trace) const
{
    return inst(st, trace);
}

util::sref<Type const> ListAppend::typeAsPipe(util::sref<SymbolTable const> st
                                            , util::sref<ListContext const>
                                            , misc::trace& trace) const
{
    return type(st, trace);
}

util::sptr<inst::Expression const> ListAppend::instAsPipe(util::sref<SymbolTable const> st
                                                        , util::sref<ListContext const>
                                                        , misc::trace& trace) const
{
    return inst(st, trace);
}

util::sref<Type const> BinaryOp::typeAsPipe(util::sref<SymbolTable const> st
                                          , util::sref<ListContext const>
                                          , misc::trace& trace) const
{
    return type(st, trace);
}

util::sptr<inst::Expression const> BinaryOp::instAsPipe(util::sref<SymbolTable const> st
                                                      , util::sref<ListContext const>
                                                      , misc::trace& trace) const
{
    return inst(st, trace);
}

util::sref<Type const> PreUnaryOp::typeAsPipe(util::sref<SymbolTable const> st
                                            , util::sref<ListContext const>
                                            , misc::trace& trace) const
{
    return type(st, trace);
}

util::sptr<inst::Expression const> PreUnaryOp::instAsPipe(util::sref<SymbolTable const> st
                                                        , util::sref<ListContext const>
                                                        , misc::trace& trace) const
{
    return inst(st, trace);
}

util::sptr<inst::Expression const> Conjunction::instAsPipe(util::sref<SymbolTable const> st
                                                         , util::sref<ListContext const>
                                                         , misc::trace& trace) const
{
    return inst(st, trace);
}

util::sptr<inst::Expression const> Disjunction::instAsPipe(util::sref<SymbolTable const> st
                                                         , util::sref<ListContext const>
                                                         , misc::trace& trace) const
{
    return inst(st, trace);
}

util::sptr<inst::Expression const> Negation::instAsPipe(util::sref<SymbolTable const> st
                                                      , util::sref<ListContext const>
                                                      , misc::trace& trace) const
{
    return inst(st, trace);
}

util::sref<Type const> ListPipeline::typeAsPipe(util::sref<SymbolTable const> st
                                              , util::sref<ListContext const>
                                              , misc::trace& trace) const
{
    return type(st, trace);
}

util::sptr<inst::Expression const> ListPipeline::instAsPipe(util::sref<SymbolTable const> st
                                                          , util::sref<ListContext const>
                                                          , misc::trace& trace) const
{
    return inst(st, trace);
}
