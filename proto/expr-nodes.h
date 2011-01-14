#ifndef __STACKENING_PROTO_EXPRESSION_NODES_H__
#define __STACKENING_PROTO_EXPRESSION_NODES_H__

#include <string>
#include <gmpxx.h>

#include "node-base.h"
#include "../instance/expr-nodes.h"
#include "../instance/scope.h"

namespace proto {

    struct func_templ;

    struct int_literal
        : public expr_base
    {
        int_literal(misc::pos_type const& ps, std::string const& image)
            : pos(ps)
            , value(image)
        {}

        inst::int_literal const* inst(inst::scope const* scope) const; 

        misc::pos_type const pos;
        mpz_class value;
    };

    struct float_literal
        : public expr_base
    {
        float_literal(misc::pos_type const& ps, std::string const& image)
            : pos(ps)
            , value(image)
        {}

        inst::float_literal const* inst(inst::scope const* scope) const; 

        misc::pos_type const pos;
        mpf_class value;
    };

    struct reference
        : public expr_base
    {
        reference(misc::pos_type const& ps, std::string const& n)
            : pos(ps)
            , name(n)
        {}

        inst::reference const* inst(inst::scope const* scope) const;

        misc::pos_type const pos;
        std::string const name;
    };

    struct call
        : public expr_base
    {
        call(misc::pos_type const& ps, func_templ* f, std::vector<expr_base const*> const& a)
            : pos(ps)
            , func(f)
            , args(a)
        {}

        inst::call const* inst(inst::scope const* scope) const;

        misc::pos_type const pos;
        func_templ* const func;
        std::vector<expr_base const*> const args;
    };

    struct binary_op
        : public expr_base
    {
        binary_op(misc::pos_type const& ps, expr_base const* l, std::string const& o, expr_base const* r)
            : pos(ps)
            , lhs(l)
            , op(o)
            , rhs(r)
        {}

        inst::binary_op const* inst(inst::scope const* scope) const;

        misc::pos_type const pos;
        expr_base const* const lhs;
        std::string const op;
        expr_base const* const rhs;
    };

    struct pre_unary_op
        : public expr_base
    {
        pre_unary_op(misc::pos_type const& ps, std::string const& o, expr_base const* r)
            : pos(ps)
            , op(o)
            , rhs(r)
        {}

        inst::pre_unary_op const* inst(inst::scope const* scope) const;

        misc::pos_type const pos;
        std::string const op;
        expr_base const* const rhs;
    };

    struct conjunction
        : public expr_base
    {
        conjunction(misc::pos_type const& ps, expr_base const* l, expr_base const* r)
            : pos(ps)
            , lhs(l)
            , rhs(r)
        {}

        inst::conjunction const* inst(inst::scope const* scope) const;

        misc::pos_type const pos;
        expr_base const* const lhs;
        expr_base const* const rhs;
    };

    struct disjunction
        : public expr_base
    {
        disjunction(misc::pos_type const& ps, expr_base const* l, expr_base const* r)
            : pos(ps)
            , lhs(l)
            , rhs(r)
        {}

        inst::disjunction const* inst(inst::scope const* scope) const;

        misc::pos_type const pos;
        expr_base const* const lhs;
        expr_base const* const rhs;
    };

    struct negation
        : public expr_base
    {
        negation(misc::pos_type const& ps, expr_base const* r)
            : pos(ps)
            , rhs(r)
        {}

        inst::negation const* inst(inst::scope const* scope) const;

        misc::pos_type const pos;
        expr_base const* const rhs;
    };

}

#endif /* __STACKENING_PROTO_EXPRESSION_NODES_H__ */
