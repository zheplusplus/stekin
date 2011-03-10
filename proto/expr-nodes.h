#ifndef __STACKENING_PROTO_EXPRESSION_NODES_H__
#define __STACKENING_PROTO_EXPRESSION_NODES_H__

#include <string>
#include <vector>
#include <gmpxx.h>

#include "node-base.h"
#include "fwd-decl.h"
#include "../instance/fwd-decl.h"

namespace proto {

    struct bool_literal
        : public expr_base
    {
        bool_literal(misc::pos_type const& ps, bool v)
            : expr_base(ps)
            , value(v)
        {}

        util::sptr<inst::expr_base const> inst(util::sref<inst::scope>) const; 

        bool const value;
    };

    struct int_literal
        : public expr_base
    {
        int_literal(misc::pos_type const& ps, std::string const& image)
            : expr_base(ps)
            , value(image)
        {}

        util::sptr<inst::expr_base const> inst(util::sref<inst::scope>) const; 

        mpz_class value;
    };

    struct float_literal
        : public expr_base
    {
        float_literal(misc::pos_type const& ps, std::string const& image)
            : expr_base(ps)
            , value(image)
        {}

        util::sptr<inst::expr_base const> inst(util::sref<inst::scope>) const; 

        mpf_class value;
    };

    struct reference
        : public expr_base
    {
        reference(misc::pos_type const& ps, std::string const& n)
            : expr_base(ps)
            , name(n)
        {}

        util::sptr<inst::expr_base const> inst(util::sref<inst::scope> scope) const;

        std::string const name;
    };

    struct call
        : public expr_base
    {
        call(misc::pos_type const& ps, util::sref<function> f, std::vector<util::sptr<expr_base const>> a)
            : expr_base(ps)
            , func(f)
            , args(std::move(a))
        {}

        util::sptr<inst::expr_base const> inst(util::sref<inst::scope> scope) const;

        util::sref<function> const func;
        std::vector<util::sptr<expr_base const>> const args;
    };

    struct binary_op
        : public expr_base
    {
        binary_op(misc::pos_type const& ps
                , util::sptr<expr_base const> l
                , std::string const& o
                , util::sptr<expr_base const> r)
            : expr_base(ps)
            , lhs(std::move(l))
            , op(o)
            , rhs(std::move(r))
        {}

        util::sptr<inst::expr_base const> inst(util::sref<inst::scope> scope) const;

        util::sptr<expr_base const> const lhs;
        std::string const op;
        util::sptr<expr_base const> const rhs;
    };

    struct pre_unary_op
        : public expr_base
    {
        pre_unary_op(misc::pos_type const& ps, std::string const& o, util::sptr<expr_base const> r)
            : expr_base(ps)
            , op(o)
            , rhs(std::move(r))
        {}

        util::sptr<inst::expr_base const> inst(util::sref<inst::scope> scope) const;

        std::string const op;
        util::sptr<expr_base const> const rhs;
    };

    struct conjunction
        : public expr_base
    {
        conjunction(misc::pos_type const& ps, util::sptr<expr_base const> l, util::sptr<expr_base const> r)
            : expr_base(ps)
            , lhs(std::move(l))
            , rhs(std::move(r))
        {}

        util::sptr<inst::expr_base const> inst(util::sref<inst::scope> scope) const;

        util::sptr<expr_base const> const lhs;
        util::sptr<expr_base const> const rhs;
    };

    struct disjunction
        : public expr_base
    {
        disjunction(misc::pos_type const& ps, util::sptr<expr_base const> l, util::sptr<expr_base const> r)
            : expr_base(ps)
            , lhs(std::move(l))
            , rhs(std::move(r))
        {}

        util::sptr<inst::expr_base const> inst(util::sref<inst::scope> scope) const;

        util::sptr<expr_base const> const lhs;
        util::sptr<expr_base const> const rhs;
    };

    struct negation
        : public expr_base
    {
        negation(misc::pos_type const& ps, util::sptr<expr_base const> r)
            : expr_base(ps)
            , rhs(std::move(r))
        {}

        util::sptr<inst::expr_base const> inst(util::sref<inst::scope> scope) const;

        util::sptr<expr_base const> const rhs;
    };

}

#endif /* __STACKENING_PROTO_EXPRESSION_NODES_H__ */
