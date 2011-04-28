#include <algorithm>

#include "test-common.h"
#include "../../flowcheck/func-body-filter.h"
#include "../../flowcheck/symbol-def-filter.h"
#include "../../flowcheck/accumulator.h"
#include "../../flowcheck/block.h"
#include "../../flowcheck/expr-nodes.h"
#include "../../flowcheck/stmt-nodes.h"
#include "../../flowcheck/function.h"
#include "../../proto/node-base.h"
#include "../../util/string.h"

using namespace test;
using namespace flchk;

namespace {

    util::sptr<proto::Statement const> nulProtoStmt()
    {
        return std::move(util::sptr<proto::Statement const>(NULL));
    }

    util::sptr<proto::Expression const> nulProtoExpr()
    {
        return std::move(util::sptr<proto::Expression const>(NULL));
    }

    util::sptr<flchk::Expression const> nulFlchkExpr()
    {
        return std::move(util::sptr<flchk::Expression const>(NULL));
    }

    struct BranchConsequence
        : public Statement
    {
        BranchConsequence(misc::position const& pos, util::sptr<Expression const> p, Block c)
            : Statement(pos)
            , predicate(std::move(p))
            , consequence(std::move(c))
        {}

        util::sptr<proto::Statement const> compile(util::sref<proto::Scope>) const
        {
            DataTree::actualOne()(pos, BRANCH_CONSQ_ONLY);
            predicate->compile(nulscope);
            DataTree::actualOne()(CONSEQUENCE);
            consequence.compile(nulscope);
            return std::move(nulProtoStmt());
        }

        util::sptr<Expression const> const predicate;
        Block const consequence;
    };

    struct BranchAlternative
        : public Statement
    {
        BranchAlternative(misc::position const& pos, util::sptr<Expression const> p, Block a)
            : Statement(pos)
            , predicate(std::move(p))
            , alternative(std::move(a))
        {}

        util::sptr<proto::Statement const> compile(util::sref<proto::Scope>) const
        {
            DataTree::actualOne()(pos, BRANCH_ALTER_ONLY);
            predicate->compile(nulscope);
            DataTree::actualOne()(ALTERNATIVE);
            alternative.compile(nulscope);
            return std::move(nulProtoStmt());
        }

        util::sptr<Expression const> const predicate;
        Block const alternative;
    };

}

void Function::compile(util::sref<proto::Scope>) const
{
    DataTree::actualOne()(pos, FUNC_DEF, name);
    std::for_each(param_names.begin()
                , param_names.end()
                , [&](std::string const& param)
                  {
                      DataTree::actualOne()(pos, PARAMETER, param);
                  });
    body.compile(nulscope);
}

void Block::addStmt(util::sptr<Statement const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

void Block::defFunc(misc::position const& pos
                  , std::string const& name
                  , std::vector<std::string> const& param_names
                  , Block body
                  , bool)
{
    _funcs.push_back(std::move(util::mkptr(new Function(pos
                                                      , name
                                                      , param_names
                                                      , std::move(body)
                                                      , false))));
}

void Block::compile(util::sref<proto::Scope>) const 
{
    DataTree::actualOne()(BLOCK_BEGIN);
    std::for_each(_funcs.begin()
                , _funcs.end()
                , [&](util::sptr<Function const> const& func)
                  {
                      func->compile(nulscope);
                  });

    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement const> const& stmt)
                  {
                      stmt->compile(nulscope);
                  });
    DataTree::actualOne()(BLOCK_END);
}

void Accumulator::addReturn(misc::position const& pos, util::sptr<Expression const> ret_val)
{
    _block.addStmt(std::move(util::mkptr(new Return(pos, std::move(ret_val)))));
}

void Accumulator::addReturnNothing(misc::position const& pos)
{
    _block.addStmt(std::move(util::mkptr(new ReturnNothing(pos))));
}

void Accumulator::addArith(misc::position const& pos, util::sptr<Expression const> expr)
{
    _block.addStmt(std::move(util::mkptr(new Arithmetics(pos, std::move(expr)))));
}

void Accumulator::addBranch(misc::position const& pos
                          , util::sptr<Expression const> predicate
                          , Accumulator consequence
                          , Accumulator alternative)
{
    _block.addStmt(std::move(util::mkptr(new Branch(pos
                                                  , std::move(predicate)
                                                  , std::move(consequence.deliver())
                                                  , std::move(alternative.deliver())))));
}

void Accumulator::addBranch(misc::position const& pos
                          , util::sptr<Expression const> predicate
                          , Accumulator consequence)
{
    _block.addStmt(std::move(util::mkptr(new BranchConsequence(pos
                                                             , std::move(predicate)
                                                             , std::move(consequence._block)))));
}

void Accumulator::addBranchAlterOnly(misc::position const& pos
                                   , util::sptr<Expression const> predicate
                                   , Accumulator alternative)
{
    _block.addStmt(std::move(util::mkptr(new BranchAlternative(pos
                                                             , std::move(predicate)
                                                             , std::move(alternative._block)))));
}

void Accumulator::defVar(misc::position const& pos
                       , std::string const& name
                       , util::sptr<Expression const> init)
{
    _block.addStmt(std::move(util::mkptr(new VarDef(pos, name, std::move(init)))));
}

void Accumulator::defFunc(misc::position const& pos
                        , std::string const& name
                        , std::vector<std::string> const& param_names
                        , Accumulator body)
{
    _block.defFunc(pos, name, param_names, std::move(body._block), false);
}

Block Accumulator::deliver()
{
    return std::move(_block);
}

void Filter::addReturn(misc::position const& pos, util::sptr<Expression const> ret_val)
{
    _accumulator.addReturn(pos, std::move(ret_val));
}

void Filter::addReturnNothing(misc::position const& pos)
{
    _accumulator.addReturnNothing(pos);
}

void Filter::addArith(misc::position const& pos, util::sptr<Expression const> expr)
{
    _accumulator.addArith(pos, std::move(expr));
}

void Filter::addBranch(misc::position const& pos
                     , util::sptr<Expression const> predicate
                     , util::sptr<Filter> consequence
                     , util::sptr<Filter> alternative)
{
    _accumulator.addBranch(pos
                         , std::move(predicate)
                         , std::move(consequence->_accumulator)
                         , std::move(alternative->_accumulator));
}

void Filter::addBranch(misc::position const& pos
                     , util::sptr<Expression const> predicate
                     , util::sptr<Filter> consequence)
{
    _accumulator.addBranch(pos, std::move(predicate), std::move(consequence->_accumulator));
}

void Filter::addBranchAlterOnly(misc::position const& pos
                              , util::sptr<Expression const> predicate
                              , util::sptr<Filter> alternative)
{
    _accumulator.addBranchAlterOnly(pos
                                  , std::move(predicate)
                                  , std::move(alternative->_accumulator));
}

void Filter::defFunc(misc::position const& pos
                   , std::string const& name
                   , std::vector<std::string> const& param_names
                   , util::sptr<Filter> body)
{
    _defFunc(pos, name, param_names, std::move(body->_accumulator));
}

Block Filter::deliver()
{
    return std::move(_accumulator.deliver());
}

void FuncBodyFilter::defVar(misc::position const& pos
                          , std::string const& name
                          , util::sptr<Expression const> init)
{
    _accumulator.defVar(pos, name, std::move(init));
}

void FuncBodyFilter::_defFunc(misc::position const& pos
                            , std::string const& name
                            , std::vector<std::string> const& param_names
                            , Accumulator body)
{
    _accumulator.defFunc(pos, name, param_names, std::move(body));
}

util::sptr<Expression const> FuncBodyFilter::makeRef(misc::position const& pos
                                                   , std::string const& name)
{
    return std::move(util::mkptr(new Reference(pos, name)));
}

util::sptr<Expression const> FuncBodyFilter::makeCall(
                    misc::position const& pos
                  , std::string const& name
                  , std::vector<util::sptr<Expression const>> args)
{
    return std::move(util::mkptr(new Call(pos, name, std::move(args))));
}

util::sptr<Expression const> FuncBodyFilter::makeFuncReference(misc::position const& pos
                                                             , std::string const& name
                                                             , int param_count)
{
    return std::move(util::mkptr(new FuncReference(pos, name, param_count)));
}

void SymbolDefFilter::defVar(misc::position const& pos
                           , std::string const& name
                           , util::sptr<Expression const> init)
{
    _accumulator.defVar(pos, name + VAR_DEF_FILTERED, std::move(init));
}

void SymbolDefFilter::_defFunc(misc::position const& pos
                             , std::string const& name
                             , std::vector<std::string> const& param_names
                             , Accumulator body)
{
    _accumulator.defFunc(pos, name + FUNC_DEF_FILTERED, param_names, std::move(body));
}

util::sptr<Expression const> SymbolDefFilter::makeRef(misc::position const& pos
                                                    , std::string const& name)
{
    return std::move(util::mkptr(new Reference(pos, name)));
}

util::sptr<Expression const> SymbolDefFilter::makeCall(
                    misc::position const& pos
                  , std::string const& name
                  , std::vector<util::sptr<Expression const>> args)
{
    return std::move(util::mkptr(new Call(pos, name, std::move(args))));
}

util::sptr<Expression const> SymbolDefFilter::makeFuncReference(misc::position const& pos
                                                              , std::string const& name
                                                              , int param_count)
{
    return std::move(util::mkptr(new FuncReference(pos, name, param_count)));
}

util::sptr<proto::Statement const> Arithmetics::compile(util::sref<proto::Scope>) const 
{
    DataTree::actualOne()(pos, ARITHMETICS);
    expr->compile(nulscope);
    return std::move(nulProtoStmt());
}

util::sptr<proto::Statement const> Branch::compile(util::sref<proto::Scope>) const 
{
    DataTree::actualOne()(pos, BRANCH);
    predicate->compile(nulscope);
    DataTree::actualOne()(CONSEQUENCE);
    consequence.compile(nulscope);
    DataTree::actualOne()(ALTERNATIVE);
    alternative.compile(nulscope);
    return std::move(nulProtoStmt());
}

util::sptr<proto::Statement const> VarDef::compile(util::sref<proto::Scope>) const 
{
    DataTree::actualOne()(pos, VAR_DEF, name);
    init->compile(nulscope);
    return std::move(nulProtoStmt());
}

util::sptr<proto::Statement const> Return::compile(util::sref<proto::Scope>) const 
{
    DataTree::actualOne()(pos, RETURN);
    ret_val->compile(nulscope);
    return std::move(nulProtoStmt());
}

util::sptr<proto::Statement const> ReturnNothing::compile(util::sref<proto::Scope>) const
{
    DataTree::actualOne()(pos, RETURN_NOTHING);
    return std::move(nulProtoStmt());
}

util::sptr<proto::Expression const> PreUnaryOp::compile(util::sref<proto::Scope>) const
{
    DataTree::actualOne()(pos, PRE_UNARY_OP, op_img);
    return std::move(nulProtoExpr());
}

util::sptr<proto::Expression const> BinaryOp::compile(util::sref<proto::Scope>) const
{
    DataTree::actualOne()(pos, BINARY_OP, op_img);
    return std::move(nulProtoExpr());
}

util::sptr<proto::Expression const> Conjunction::compile(util::sref<proto::Scope>) const
{
    DataTree::actualOne()(pos, BINARY_OP, "&&");
    return std::move(nulProtoExpr());
}

util::sptr<proto::Expression const> Disjunction::compile(util::sref<proto::Scope>) const
{
    DataTree::actualOne()(pos, BINARY_OP, "||");
    return std::move(nulProtoExpr());
}

util::sptr<proto::Expression const> Negation::compile(util::sref<proto::Scope>) const
{
    DataTree::actualOne()(pos, PRE_UNARY_OP, "!");
    return std::move(nulProtoExpr());
}

util::sptr<proto::Expression const> Reference::compile(util::sref<proto::Scope>) const
{
    DataTree::actualOne()(pos, REFERENCE, name);
    return std::move(nulProtoExpr());
}

util::sptr<proto::Expression const> BoolLiteral::compile(util::sref<proto::Scope>) const
{
    DataTree::actualOne()(pos, BOOLEAN, util::str(value));
    return std::move(nulProtoExpr());
}

util::sptr<proto::Expression const> IntLiteral::compile(util::sref<proto::Scope>) const
{
    DataTree::actualOne()(pos, INTEGER, util::str(value));
    return std::move(nulProtoExpr());
}

util::sptr<proto::Expression const> FloatLiteral::compile(util::sref<proto::Scope>) const
{
    DataTree::actualOne()(pos, FLOATING, util::str(value));
    return std::move(nulProtoExpr());
}

util::sptr<proto::Expression const> Call::compile(util::sref<proto::Scope> s) const
{
    DataTree::actualOne()(pos, CALL, name, args.size());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& arg)
                  {
                      arg->compile(s);
                  });
    return std::move(nulProtoExpr());
}

util::sptr<proto::Expression const> FuncReference::compile(util::sref<proto::Scope>) const
{
    DataTree::actualOne()(pos, FUNC_REFERENCE, name, param_count);
    return std::move(nulProtoExpr());
}

bool Expression::isLiteral() const
{
    return false;
}

bool Expression::boolValue() const
{
    return false;
}

util::sptr<Expression const> Expression::operate(misc::position const&
                                               , std::string const&
                                               , mpz_class const&) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> Expression::operate(misc::position const&
                                               , std::string const&
                                               , mpf_class const&) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> Expression::operate(misc::position const&
                                               , std::string const&
                                               , bool) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> Expression::asRHS(misc::position const&
                                             , std::string const&
                                             , util::sptr<Expression const>) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> Expression::asRHS(misc::position const&, std::string const&) const
{
    return std::move(nulFlchkExpr());
}

bool PreUnaryOp::isLiteral() const
{
    return false;
}

bool PreUnaryOp::boolValue() const
{
    return false;
}

std::string PreUnaryOp::typeName() const
{
    return "";
}

util::sptr<Expression const> PreUnaryOp::fold() const
{
    return std::move(nulFlchkExpr());
}

bool BinaryOp::isLiteral() const
{
    return false;
}

bool BinaryOp::boolValue() const
{
    return false;
}

std::string BinaryOp::typeName() const
{
    return "";
}

util::sptr<Expression const> BinaryOp::fold() const
{
    return std::move(nulFlchkExpr());
}

bool Conjunction::isLiteral() const
{
    return false;
}

bool Conjunction::boolValue() const
{
    return false;
}

std::string Conjunction::typeName() const
{
    return "";
}

util::sptr<Expression const> Conjunction::fold() const
{
    return std::move(nulFlchkExpr());
}

bool Disjunction::isLiteral() const
{
    return false;
}

bool Disjunction::boolValue() const
{
    return false;
}

std::string Disjunction::typeName() const
{
    return "";
}

util::sptr<Expression const> Disjunction::fold() const
{
    return std::move(nulFlchkExpr());
}

bool Negation::isLiteral() const
{
    return false;
}

bool Negation::boolValue() const
{
    return false;
}

std::string Negation::typeName() const
{
    return "";
}

util::sptr<Expression const> Negation::fold() const
{
    return std::move(nulFlchkExpr());
}

std::string Reference::typeName() const
{
    return "";
}

util::sptr<Expression const> Reference::fold() const
{
    return std::move(nulFlchkExpr());
}

bool BoolLiteral::isLiteral() const
{
    return false;
}

bool BoolLiteral::boolValue() const
{
    return false;
}

std::string BoolLiteral::typeName() const
{
    return "";
}

util::sptr<Expression const> BoolLiteral::fold() const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> BoolLiteral::operate(misc::position const&
                                                , std::string const&
                                                , mpz_class const&) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> BoolLiteral::operate(misc::position const&
                                                , std::string const&
                                                , mpf_class const&) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> BoolLiteral::operate(misc::position const&
                                                , std::string const&
                                                , bool) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> BoolLiteral::asRHS(misc::position const&
                                              , std::string const&
                                              , util::sptr<Expression const>) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> BoolLiteral::asRHS(misc::position const&, std::string const&) const
{
    return std::move(nulFlchkExpr());
}

bool IntLiteral::isLiteral() const
{
    return false;
}

bool IntLiteral::boolValue() const
{
    return false;
}

std::string IntLiteral::typeName() const
{
    return "";
}

util::sptr<Expression const> IntLiteral::fold() const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> IntLiteral::operate(misc::position const&
                                               , std::string const&
                                               , mpz_class const&) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> IntLiteral::operate(misc::position const&
                                               , std::string const&
                                               , mpf_class const&) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> IntLiteral::operate(misc::position const&
                                               , std::string const&
                                               , bool) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> IntLiteral::asRHS(misc::position const&
                                             , std::string const&
                                             , util::sptr<Expression const>) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> IntLiteral::asRHS(misc::position const&, std::string const&) const
{
    return std::move(nulFlchkExpr());
}

bool FloatLiteral::isLiteral() const
{
    return false;
}

bool FloatLiteral::boolValue() const
{
    return false;
}

std::string FloatLiteral::typeName() const
{
    return "";
}

util::sptr<Expression const> FloatLiteral::fold() const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> FloatLiteral::operate(misc::position const&
                                                 , std::string const&
                                                 , mpz_class const&) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> FloatLiteral::operate(misc::position const&
                                                 , std::string const&
                                                 , mpf_class const&) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> FloatLiteral::operate(misc::position const&
                                                 , std::string const&
                                                 , bool) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> FloatLiteral::asRHS(misc::position const&
                                               , std::string const&
                                               , util::sptr<Expression const>) const
{
    return std::move(nulFlchkExpr());
}

util::sptr<Expression const> FloatLiteral::asRHS(misc::position const&, std::string const&) const
{
    return std::move(nulFlchkExpr());
}

std::string Call::typeName() const
{
    return "";
}

util::sptr<Expression const> Call::fold() const
{
    return std::move(nulFlchkExpr());
}

std::string FuncReference::typeName() const
{
    return "";
}

util::sptr<Expression const> FuncReference::fold() const
{
    return std::move(nulFlchkExpr());
}
