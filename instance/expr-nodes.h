#ifndef __STEKIN_INSTANCE_EXPRESSION_NODES_H__
#define __STEKIN_INSTANCE_EXPRESSION_NODES_H__

#include <vector>

#include "node-base.h"
#include "../util/pointer.h"
#include "../misc/platform.h"
#include "../misc/pos-type.h"

namespace inst {

    struct IntLiteral
        : public Expression
    {
        explicit IntLiteral(platform::i4_type v)
            : value(v)
        {}

        void write() const;

        platform::i4_type const value;
    };

    struct FloatLiteral
        : public Expression
    {
        explicit FloatLiteral(platform::f8_type v)
            : value(v)
        {}

        void write() const;

        platform::f8_type const value;
    };

    struct BoolLiteral
        : public Expression
    {
        explicit BoolLiteral(bool v)
            : value(v)
        {}

        void write() const;

        bool const value;
    };

    struct Reference
        : public Expression
    {
        Reference(std::string const& en, int l, int off)
            : type_exported_name(en)
            , level(l)
            , stack_offset(off)
        {}

        void write() const;

        std::string const type_exported_name;
        int const level;
        int const stack_offset;
    };

    struct Call
        : public Expression
    {
        Call(util::id c, std::vector<util::sptr<Expression const>> a)
            : call_id(c)
            , args(std::move(a))
        {}

        void write() const;

        util::id const call_id;
        std::vector<util::sptr<Expression const>> args;
    };

    struct FuncReference
        : public Expression
    {
        struct ArgInfo {
            int const level;
            int const ref_offset;
            std::string const exported_name;
            int const self_offset;

            ArgInfo(int l, int roff, std::string const& en, int soff)
                : level(l)
                , ref_offset(roff)
                , exported_name(en)
                , self_offset(soff)
            {}
        };

        FuncReference(int s, std::vector<ArgInfo> const& a)
            : size(s)
            , args(a)
        {}

        void write() const;

        int const size;
        std::vector<ArgInfo> const args;
    };

    struct BinaryOp
        : public Expression
    {
        BinaryOp(util::sptr<Expression const> l
               , std::string const& o
               , util::sptr<Expression const> r)
            : lhs(std::move(l))
            , op(o)
            , rhs(std::move(r))
        {}

        void write() const;

        util::sptr<Expression const> const lhs;
        std::string const op;
        util::sptr<Expression const> const rhs;
    };

    struct PreUnaryOp
        : public Expression
    {
        PreUnaryOp(std::string const& o, util::sptr<Expression const> r)
            : op(o)
            , rhs(std::move(r))
        {}

        void write() const;

        std::string const op;
        util::sptr<Expression const> const rhs;
    };

    struct Conjunction
        : public Expression
    {
        Conjunction(util::sptr<Expression const> l, util::sptr<Expression const> r)
            : lhs(std::move(l))
            , rhs(std::move(r))
        {}

        void write() const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct Disjunction
        : public Expression
    {
        Disjunction(util::sptr<Expression const> l, util::sptr<Expression const> r)
            : lhs(std::move(l))
            , rhs(std::move(r))
        {}

        void write() const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct Negation
        : public Expression
    {
        explicit Negation(util::sptr<Expression const> r)
            : rhs(std::move(r))
        {}

        void write() const;

        util::sptr<Expression const> const rhs;
    };

}

#endif /* __STEKIN_INSTANCE_EXPRESSION_NODES_H__ */
