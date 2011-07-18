#ifndef __STEKIN_INSTANCE_EXPRESSION_NODES_H__
#define __STEKIN_INSTANCE_EXPRESSION_NODES_H__

#include <vector>

#include "node-base.h"
#include "address.h"
#include "../util/pointer.h"
#include "../util/sn.h"
#include "../misc/platform.h"
#include "../misc/pos-type.h"

namespace inst {

    struct IntLiteral
        : public Expression
    {
        explicit IntLiteral(platform::int_type v)
            : value(v)
        {}

        void write() const;

        platform::int_type const value;
    };

    struct FloatLiteral
        : public Expression
    {
        explicit FloatLiteral(platform::float_type v)
            : value(v)
        {}

        void write() const;

        platform::float_type const value;
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
        Reference(std::string const& en, Address const& a)
            : type_exported_name(en)
            , address(a)
        {}

        void write() const;

        std::string const type_exported_name;
        Address address;
    };

    struct Call
        : public Expression
    {
        Call(util::serial_num c, std::vector<util::sptr<Expression const>> a)
            : call_sn(c)
            , args(std::move(a))
        {}

        void write() const;

        util::serial_num const call_sn;
        std::vector<util::sptr<Expression const>> args;
    };

    struct FuncReference
        : public Expression
    {
        struct ArgInfo {
            Address address;
            std::string const exported_name;
            int const self_offset;

            ArgInfo(Address const& a, std::string const& en, int soff)
                : address(a)
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
