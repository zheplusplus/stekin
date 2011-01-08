#ifndef __STACKENING_INSTANCE_EXPRESSION_NODES_H__
#define __STACKENING_INSTANCE_EXPRESSION_NODES_H__

#include "node-base.h"
#include "operation.h"
#include "variable.h"
#include "../misc/platform.h"

namespace inst {

    struct function;

    struct int_literal
        : public expr_base
    {
        explicit int_literal(platform::i4_type v)
            : value(v)
        {}

        type const* typeof() const;

        platform::i4_type const value;
    };

    struct float_literal
        : public expr_base
    {
        explicit float_literal(platform::f8_type v)
            : value(v)
        {}

        type const* typeof() const;

        platform::f8_type const value;
    };

    struct bool_literal
        : public expr_base
    {
        explicit bool_literal(bool v)
            : value(v)
        {}

        type const* typeof() const;

        bool const value;
    };

    struct reference
        : public expr_base
    {
        explicit reference(variable const& v)
            : var(v)
        {}

        type const* typeof() const;

        variable const var;
    };

    struct call
        : public expr_base
    {
        call(function const* f, std::vector<expr_base const*> const& a)
            : func(f)
            , args(a)
        {}

        type const* typeof() const;

        function const* const func;
        std::vector<expr_base const*> args;
    };

    struct binary_op
        : public expr_base
    {
        binary_op(expr_base const* l, operation const* o, expr_base const* r)
            : lhs(l)
            , op(o)
            , rhs(r)
        {}

        type const* typeof() const;

        expr_base const* const lhs;
        operation const* const op;
        expr_base const* const rhs;
    };

    struct pre_unary_op
        : public expr_base
    {
        pre_unary_op(operation const* o, expr_base const* r)
            : op(o)
            , rhs(r)
        {}

        type const* typeof() const;

        operation const* const op;
        expr_base const* const rhs;
    };

    struct conjunction
        : public expr_base
    {
        conjunction(expr_base const* l, expr_base const* r)
            : lhs(l)
            , rhs(r)
        {}

        type const* typeof() const;

        expr_base const* const lhs;
        expr_base const* const rhs;
    };

    struct disjunction
        : public expr_base
    {
        disjunction(expr_base const* l, expr_base const* r)
            : lhs(l)
            , rhs(r)
        {}

        type const* typeof() const;

        expr_base const* const lhs;
        expr_base const* const rhs;
    };

    struct negation
        : public expr_base
    {
        explicit negation(expr_base const* r)
            : rhs(r)
        {}

        type const* typeof() const;

        expr_base const* const rhs;
    };

}

#endif /* __STACKENING_INSTANCE_EXPRESSION_NODES_H__ */
