#ifndef __STEKIN_PROTO_EXPRESSION_NODES_H__
#define __STEKIN_PROTO_EXPRESSION_NODES_H__

#include <string>
#include <vector>
#include <map>
#include <gmpxx.h>

#include "node-base.h"
#include "fwd-decl.h"
#include "func-reference-type.h"
#include "../instance/fwd-decl.h"
#include "../util/map-compare.h"

namespace proto {

    struct BoolLiteral
        : public Expression
    {
        BoolLiteral(misc::position const& pos, bool v)
            : Expression(pos)
            , value(v)
        {}

        util::sref<inst::Type const> type(util::sref<SymbolTable const>);
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const>); 

        bool const value;
    };

    struct IntLiteral
        : public Expression
    {
        IntLiteral(misc::position const& pos, mpz_class const& v)
            : Expression(pos)
            , value(v)
        {}

        util::sref<inst::Type const> type(util::sref<SymbolTable const>);
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const>); 

        mpz_class value;
    };

    struct FloatLiteral
        : public Expression
    {
        FloatLiteral(misc::position const& pos, mpf_class const& v)
            : Expression(pos)
            , value(v)
        {}

        util::sref<inst::Type const> type(util::sref<SymbolTable const>);
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const>); 

        mpf_class value;
    };

    struct Reference
        : public Expression
    {
        Reference(misc::position const& pos, std::string const& n)
            : Expression(pos)
            , name(n)
        {}

        util::sref<inst::Type const> type(util::sref<SymbolTable const> scope);
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> scope);

        std::string const name;
    };

    struct Call
        : public Expression
    {
        Call(misc::position const& pos
           , util::sref<Function> f
           , std::vector<util::sptr<Expression>> a)
                : Expression(pos)
                , func(f)
                , args(std::move(a))
        {}

        util::sref<inst::Type const> type(util::sref<SymbolTable const> scope);
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> scope);

        util::sref<Function> const func;
        std::vector<util::sptr<Expression>> const args;
    };

    struct Functor
        : public Expression
    {
        Functor(misc::position const& pos
              , std::string const& n
              , std::vector<util::sptr<Expression>> a)
            : Expression(pos)
            , name(n)
            , args(std::move(a))
        {}

        util::sref<inst::Type const> type(util::sref<SymbolTable const> scope);
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> scope);

        std::string const name;
        std::vector<util::sptr<Expression>> const args;
    };

    struct FuncReference
        : public Expression
    {
        FuncReference(misc::position const& pos, util::sref<Function> f)
            : Expression(pos)
            , _func(f)
        {}

        util::sref<inst::Type const> type(util::sref<SymbolTable const> scope);
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> scope);
    private:
        util::sref<FuncReferenceType const> _mkType(util::sref<SymbolTable const> scope);
    private:
        util::sref<Function> const _func;
        std::map<std::map<std::string, inst::Variable const>
               , FuncReferenceType const
               , util::map_less_t> _type_cache;
    };

    struct BinaryOp
        : public Expression
    {
        BinaryOp(misc::position const& pos
                , util::sptr<Expression> l
                , std::string const& o
                , util::sptr<Expression> r)
            : Expression(pos)
            , lhs(std::move(l))
            , op(o)
            , rhs(std::move(r))
        {}

        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> scope);

        util::sptr<Expression> const lhs;
        std::string const op;
        util::sptr<Expression> const rhs;
    };

    struct PreUnaryOp
        : public Expression
    {
        PreUnaryOp(misc::position const& pos, std::string const& o, util::sptr<Expression> r)
            : Expression(pos)
            , op(o)
            , rhs(std::move(r))
        {}

        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> scope);

        std::string const op;
        util::sptr<Expression> const rhs;
    };

    struct Conjunction
        : public Expression
    {
        Conjunction(misc::position const& pos
                  , util::sptr<Expression> l
                  , util::sptr<Expression> r)
            : Expression(pos)
            , lhs(std::move(l))
            , rhs(std::move(r))
        {}

        util::sref<inst::Type const> type(util::sref<SymbolTable const>);
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> scope);

        util::sptr<Expression> const lhs;
        util::sptr<Expression> const rhs;
    };

    struct Disjunction
        : public Expression
    {
        Disjunction(misc::position const& pos
                  , util::sptr<Expression> l
                  , util::sptr<Expression> r)
            : Expression(pos)
            , lhs(std::move(l))
            , rhs(std::move(r))
        {}

        util::sref<inst::Type const> type(util::sref<SymbolTable const>);
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> scope);

        util::sptr<Expression> const lhs;
        util::sptr<Expression> const rhs;
    };

    struct Negation
        : public Expression
    {
        Negation(misc::position const& pos, util::sptr<Expression> r)
            : Expression(pos)
            , rhs(std::move(r))
        {}

        util::sref<inst::Type const> type(util::sref<SymbolTable const>);
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> scope);

        util::sptr<Expression> const rhs;
    };

}

#endif /* __STEKIN_PROTO_EXPRESSION_NODES_H__ */
