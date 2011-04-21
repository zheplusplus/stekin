#ifndef __STEKIN_INSTANCE_EXPRESSION_NODES_H__
#define __STEKIN_INSTANCE_EXPRESSION_NODES_H__

#include <vector>

#include "node-base.h"
#include "variable.h"
#include "func-reference-type.h"
#include "fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/platform.h"
#include "../proto/fwd-decl.h"

namespace inst {

    struct IntLiteral
        : public Expression
    {
        explicit IntLiteral(platform::i4_type v)
            : value(v)
        {}

        util::sref<type const> typeof() const;
        void write() const;

        platform::i4_type const value;
    };

    struct FloatLiteral
        : public Expression
    {
        explicit FloatLiteral(platform::f8_type v)
            : value(v)
        {}

        util::sref<type const> typeof() const;
        void write() const;

        platform::f8_type const value;
    };

    struct BoolLiteral
        : public Expression
    {
        explicit BoolLiteral(bool v)
            : value(v)
        {}

        util::sref<type const> typeof() const;
        void write() const;

        bool const value;
    };

    struct reference
        : public Expression
    {
        explicit reference(variable const& v)
            : var(v)
        {}

        util::sref<type const> typeof() const;
        void write() const;

        variable const var;
    };

    struct call
        : public Expression
    {
        call(util::sref<Function const> f, std::vector<util::sptr<Expression const>> a)
            : func(f)
            , args(std::move(a))
        {}

        util::sref<type const> typeof() const;
        void write() const;

        util::sref<Function const> const func;
        std::vector<util::sptr<Expression const>> args;
    };

    struct func_reference
        : public Expression
    {
        func_reference(misc::position const& reference_pos
                     , util::sref<proto::Function> func_proto
                     , int level
                     , std::map<std::string, variable const> const& cr)
            : _type(reference_pos, func_proto, level, cr)
        {}

        util::sref<type const> typeof() const;
        void write() const;
    private:
        func_reference_type const _type;
    };

    struct binary_op
        : public Expression
    {
        binary_op(util::sptr<Expression const> l, operation const* o, util::sptr<Expression const> r)
            : lhs(std::move(l))
            , op(o)
            , rhs(std::move(r))
        {}

        util::sref<type const> typeof() const;
        void write() const;

        util::sptr<Expression const> const lhs;
        operation const* const op;
        util::sptr<Expression const> const rhs;
    };

    struct pre_unary_op
        : public Expression
    {
        pre_unary_op(operation const* o, util::sptr<Expression const> r)
            : op(o)
            , rhs(std::move(r))
        {}

        util::sref<type const> typeof() const;
        void write() const;

        operation const* const op;
        util::sptr<Expression const> const rhs;
    };

    struct conjunction
        : public Expression
    {
        conjunction(misc::position const& p, util::sptr<Expression const> l, util::sptr<Expression const> r);

        util::sref<type const> typeof() const;
        void write() const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct disjunction
        : public Expression
    {
        disjunction(misc::position const& p, util::sptr<Expression const> l, util::sptr<Expression const> r);

        util::sref<type const> typeof() const;
        void write() const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct negation
        : public Expression
    {
        explicit negation(misc::position const& p, util::sptr<Expression const> r);

        util::sref<type const> typeof() const;
        void write() const;

        util::sptr<Expression const> const rhs;
    };

}

#endif /* __STEKIN_INSTANCE_EXPRESSION_NODES_H__ */
