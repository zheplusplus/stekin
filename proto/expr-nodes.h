#ifndef __STEKIN_PROTO_EXPRESSION_NODES_H__
#define __STEKIN_PROTO_EXPRESSION_NODES_H__

#include <string>
#include <vector>
#include <gmpxx.h>

#include "node-base.h"
#include "fwd-decl.h"
#include "../instance/fwd-decl.h"

namespace proto {

    struct BoolLiteral
        : public Expression
    {
        BoolLiteral(misc::position const& pos, bool v)
            : Expression(pos)
            , value(v)
        {}

        util::sptr<inst::Expression const> inst(util::sref<inst::Scope>) const; 

        bool const value;
    };

    struct IntLiteral
        : public Expression
    {
        IntLiteral(misc::position const& pos, mpz_class const& v)
            : Expression(pos)
            , value(v)
        {}

        util::sptr<inst::Expression const> inst(util::sref<inst::Scope>) const; 

        mpz_class value;
    };

    struct FloatLiteral
        : public Expression
    {
        FloatLiteral(misc::position const& pos, mpf_class const& v)
            : Expression(pos)
            , value(v)
        {}

        util::sptr<inst::Expression const> inst(util::sref<inst::Scope>) const; 

        mpf_class value;
    };

    struct Reference
        : public Expression
    {
        Reference(misc::position const& pos, std::string const& n)
            : Expression(pos)
            , name(n)
        {}

        util::sptr<inst::Expression const> inst(util::sref<inst::Scope> scope) const;

        std::string const name;
    };

    struct Call
        : public Expression
    {
        Call(misc::position const& pos
           , util::sref<Function> f
           , std::vector<util::sptr<Expression const>> a)
                : Expression(pos)
                , func(f)
                , args(std::move(a))
        {}

        util::sptr<inst::Expression const> inst(util::sref<inst::Scope> scope) const;

        util::sref<Function> const func;
        std::vector<util::sptr<Expression const>> const args;
    };

    struct Functor
        : public Expression
    {
        Functor(misc::position const& pos
              , std::string const& n
              , std::vector<util::sptr<Expression const>> a)
            : Expression(pos)
            , name(n)
            , args(std::move(a))
        {}

        util::sptr<inst::Expression const> inst(util::sref<inst::Scope> scope) const;

        std::string const name;
        std::vector<util::sptr<Expression const>> const args;
    };

    struct FuncReference
        : public Expression
    {
        FuncReference(misc::position const& pos, util::sref<Function> f)
            : Expression(pos)
            , func(f)
        {}

        util::sptr<inst::Expression const> inst(util::sref<inst::Scope> scope) const;

        util::sref<Function> const func;
    };

    struct BinaryOp
        : public Expression
    {
        BinaryOp(misc::position const& pos
                , util::sptr<Expression const> l
                , std::string const& o
                , util::sptr<Expression const> r)
            : Expression(pos)
            , lhs(std::move(l))
            , op(o)
            , rhs(std::move(r))
        {}

        util::sptr<inst::Expression const> inst(util::sref<inst::Scope> scope) const;

        util::sptr<Expression const> const lhs;
        std::string const op;
        util::sptr<Expression const> const rhs;
    };

    struct PreUnaryOp
        : public Expression
    {
        PreUnaryOp(misc::position const& pos, std::string const& o, util::sptr<Expression const> r)
            : Expression(pos)
            , op(o)
            , rhs(std::move(r))
        {}

        util::sptr<inst::Expression const> inst(util::sref<inst::Scope> scope) const;

        std::string const op;
        util::sptr<Expression const> const rhs;
    };

    struct Conjunction
        : public Expression
    {
        Conjunction(misc::position const& pos
                  , util::sptr<Expression const> l
                  , util::sptr<Expression const> r)
            : Expression(pos)
            , lhs(std::move(l))
            , rhs(std::move(r))
        {}

        util::sptr<inst::Expression const> inst(util::sref<inst::Scope> scope) const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct Disjunction
        : public Expression
    {
        Disjunction(misc::position const& pos
                  , util::sptr<Expression const> l
                  , util::sptr<Expression const> r)
            : Expression(pos)
            , lhs(std::move(l))
            , rhs(std::move(r))
        {}

        util::sptr<inst::Expression const> inst(util::sref<inst::Scope> scope) const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct Negation
        : public Expression
    {
        Negation(misc::position const& pos, util::sptr<Expression const> r)
            : Expression(pos)
            , rhs(std::move(r))
        {}

        util::sptr<inst::Expression const> inst(util::sref<inst::Scope> scope) const;

        util::sptr<Expression const> const rhs;
    };

}

#endif /* __STEKIN_PROTO_EXPRESSION_NODES_H__ */
