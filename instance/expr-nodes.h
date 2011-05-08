#ifndef __STEKIN_INSTANCE_EXPRESSION_NODES_H__
#define __STEKIN_INSTANCE_EXPRESSION_NODES_H__

#include <vector>

#include "node-base.h"
#include "fwd-decl.h"
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

        util::sref<Type const> typeof() const;
        void write() const;

        platform::i4_type const value;
    };

    struct FloatLiteral
        : public Expression
    {
        explicit FloatLiteral(platform::f8_type v)
            : value(v)
        {}

        util::sref<Type const> typeof() const;
        void write() const;

        platform::f8_type const value;
    };

    struct BoolLiteral
        : public Expression
    {
        explicit BoolLiteral(bool v)
            : value(v)
        {}

        util::sref<Type const> typeof() const;
        void write() const;

        bool const value;
    };

    struct Reference
        : public Expression
    {
        Reference(util::sref<Type const> t, int l, int off)
            : type(t)
            , level(l)
            , stack_offset(off)
        {}

        util::sref<Type const> typeof() const;
        void write() const;

        util::sref<Type const> const type;
        int const level;
        int const stack_offset;
    };

    struct Call
        : public Expression
    {
        Call(util::sref<Type const> t, util::id c, std::vector<util::sptr<Expression const>> a)
            : type(t)
            , call_id(c)
            , args(std::move(a))
        {}

        util::sref<Type const> typeof() const;
        void write() const;

        util::sref<Type const> const type;
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

        FuncReference(int s, std::vector<ArgInfo> const& a, util::sref<Type const> t)
            : size(s)
            , args(a)
            , type(t)
        {}

        util::sref<Type const> typeof() const;
        void write() const;

        int const size;
        std::vector<ArgInfo> const args;
        util::sref<Type const> type;
    };

    struct BinaryOp
        : public Expression
    {
        BinaryOp(util::sptr<Expression const> l
               , std::string const& o
               , util::sref<Type const> t
               , util::sptr<Expression const> r)
            : lhs(std::move(l))
            , op(o)
            , type(t)
            , rhs(std::move(r))
        {}

        util::sref<Type const> typeof() const;
        void write() const;

        util::sptr<Expression const> const lhs;
        std::string const op;
        util::sref<Type const> const type;
        util::sptr<Expression const> const rhs;
    };

    struct PreUnaryOp
        : public Expression
    {
        PreUnaryOp(std::string const& o, util::sref<Type const> t , util::sptr<Expression const> r)
            : op(o)
            , type(t)
            , rhs(std::move(r))
        {}

        util::sref<Type const> typeof() const;
        void write() const;

        std::string const op;
        util::sref<Type const> type;
        util::sptr<Expression const> const rhs;
    };

    struct Conjunction
        : public Expression
    {
        Conjunction(misc::position const& p
                  , util::sptr<Expression const> l
                  , util::sptr<Expression const> r);

        util::sref<Type const> typeof() const;
        void write() const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct Disjunction
        : public Expression
    {
        Disjunction(misc::position const& p
                  , util::sptr<Expression const> l
                  , util::sptr<Expression const> r);

        util::sref<Type const> typeof() const;
        void write() const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct Negation
        : public Expression
    {
        Negation(misc::position const& p, util::sptr<Expression const> r);

        util::sref<Type const> typeof() const;
        void write() const;

        util::sptr<Expression const> const rhs;
    };

}

#endif /* __STEKIN_INSTANCE_EXPRESSION_NODES_H__ */
