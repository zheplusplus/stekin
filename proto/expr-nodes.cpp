#include <algorithm>

#include "expr-nodes.h"
#include "function.h"
#include "../instance/expr-nodes.h"
#include "../instance/symbol-table.h"
#include "../instance/function.h"

using namespace proto;

util::sref<inst::Type const> BoolLiteral::type(util::sref<inst::SymbolTable const>)
{
    return inst::Type::BIT_BOOL;
}

util::sptr<inst::Expression const> BoolLiteral::inst(util::sref<inst::SymbolTable const>)
{
    return util::mkptr(new inst::BoolLiteral(value));
}

util::sref<inst::Type const> IntLiteral::type(util::sref<inst::SymbolTable const>)
{
    return inst::Type::BIT_INT;
}

util::sptr<inst::Expression const> IntLiteral::inst(util::sref<inst::SymbolTable const>)
{
    return util::mkptr(new inst::IntLiteral(value.get_si()));
}

util::sref<inst::Type const> FloatLiteral::type(util::sref<inst::SymbolTable const>)
{
    return inst::Type::BIT_FLOAT;
}

util::sptr<inst::Expression const> FloatLiteral::inst(util::sref<inst::SymbolTable const>)
{
    return util::mkptr(new inst::FloatLiteral(value.get_d()));
}

util::sref<inst::Type const> Reference::type(util::sref<inst::SymbolTable const> st)
{
    return st->queryVar(pos, name).type;
}

util::sptr<inst::Expression const> Reference::inst(util::sref<inst::SymbolTable const> st)
{
    return util::mkptr(new inst::Reference(st->queryVar(pos, name)));
}

template <typename _CallMaker>
static util::sptr<inst::Expression const> instantiateFunction(
            std::vector<util::sptr<Expression>> const& args
          , util::sref<inst::SymbolTable const> st
          , _CallMaker call_maker)
{
    std::vector<util::sptr<inst::Expression const>> arg_instances;
    std::vector<util::sref<inst::Type const>> arg_types;
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression> const& expr)
                  {
                      util::sptr<inst::Expression const> arg(expr->inst(st));
                      arg_types.push_back(arg->typeof());
                      arg_instances.push_back(std::move(arg));
                  });
    return std::move(call_maker(arg_types, std::move(arg_instances)));
}

util::sref<inst::Type const> Call::type(util::sref<inst::SymbolTable const> st)
{
    std::vector<util::sref<inst::Type const>> arg_types;
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression> const& arg)
                  {
                      arg_types.push_back(arg->inst(st)->typeof());
                  });
    return func->inst(pos, st, arg_types)->getReturnType();
}

util::sptr<inst::Expression const> Call::inst(util::sref<inst::SymbolTable const> st)
{
    return instantiateFunction(
                args
              , st
              , [&](std::vector<util::sref<inst::Type const>> const& arg_types
                  , std::vector<util::sptr<inst::Expression const>> args)
                {
                    return util::mkptr(new inst::Call(func->inst(pos, st, arg_types)
                                                    , std::move(args)));
                });
}

util::sref<inst::Type const> Functor::type(util::sref<inst::SymbolTable const> st)
{
    return inst(st)->typeof();
}

util::sptr<inst::Expression const> Functor::inst(util::sref<inst::SymbolTable const> st)
{
    return instantiateFunction(
                args
              , st
              , [&](std::vector<util::sref<inst::Type const>> const& arg_types
                  , std::vector<util::sptr<inst::Expression const>> args)
                {
                    return st->queryVar(pos, name).call(pos, arg_types, std::move(args));
                });
}

util::sref<inst::Type const> FuncReference::type(util::sref<inst::SymbolTable const> st)
{
    return _mkType(st);
}

util::sptr<inst::Expression const> FuncReference::inst(util::sref<inst::SymbolTable const> st)
{
    return util::mkptr(new inst::FuncReference(_mkType(st)));
}

util::sref<FuncReferenceType const> FuncReference::_mkType(util::sref<inst::SymbolTable const> st)
{
    std::map<std::string, inst::Variable const> ext_vars(_func->bindExternalVars(pos, st));
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

util::sptr<inst::Expression const> BinaryOp::inst(util::sref<inst::SymbolTable const> st)
{
    util::sptr<inst::Expression const> left = lhs->inst(st);
    util::sptr<inst::Expression const> right = rhs->inst(st);
    util::sref<inst::Type const> ltype = left->typeof();
    util::sref<inst::Type const> rtype = right->typeof();
    return util::mkptr(new inst::BinaryOp(std::move(left)
                                        , st->queryBinary(pos, op, ltype, rtype)
                                        , std::move(right)));
}

util::sptr<inst::Expression const> PreUnaryOp::inst(util::sref<inst::SymbolTable const> st)
{
    util::sptr<inst::Expression const> right = rhs->inst(st);
    util::sref<inst::Type const> rtype = right->typeof();
    return util::mkptr(new inst::PreUnaryOp(st->queryPreUnary(pos, op, rtype), std::move(right)));
}

util::sref<inst::Type const> Conjunction::type(util::sref<inst::SymbolTable const>)
{
    return inst::Type::BIT_BOOL;
}

util::sptr<inst::Expression const> Conjunction::inst(util::sref<inst::SymbolTable const> st)
{
    return util::mkptr(new inst::Conjunction(pos, lhs->inst(st), rhs->inst(st)));
}

util::sref<inst::Type const> Disjunction::type(util::sref<inst::SymbolTable const>)
{
    return inst::Type::BIT_BOOL;
}

util::sptr<inst::Expression const> Disjunction::inst(util::sref<inst::SymbolTable const> st)
{
    return util::mkptr(new inst::Disjunction(pos, lhs->inst(st), rhs->inst(st)));
}

util::sref<inst::Type const> Negation::type(util::sref<inst::SymbolTable const>)
{
    return inst::Type::BIT_BOOL;
}

util::sptr<inst::Expression const> Negation::inst(util::sref<inst::SymbolTable const> st)
{
    return util::mkptr(new inst::Negation(pos, rhs->inst(st)));
}
