#ifndef __STACKENING_INSTANCE_EXPRESSION_NODES_H__
#define __STACKENING_INSTANCE_EXPRESSION_NODES_H__

#include <vector>

#include "node-base.h"
#include "variable.h"
#include "fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/platform.h"

namespace inst {

    struct int_literal
        : public expr_base
    {
        explicit int_literal(platform::i4_type v)
            : value(v)
        {}

        type const* typeof() const;
        void write() const;

        platform::i4_type const value;
    };

    struct float_literal
        : public expr_base
    {
        explicit float_literal(platform::f8_type v)
            : value(v)
        {}

        type const* typeof() const;
        void write() const;

        platform::f8_type const value;
    };

    struct bool_literal
        : public expr_base
    {
        explicit bool_literal(bool v)
            : value(v)
        {}

        type const* typeof() const;
        void write() const;

        bool const value;
    };

    struct reference
        : public expr_base
    {
        explicit reference(variable const& v)
            : var(v)
        {}

        type const* typeof() const;
        void write() const;

        variable const var;
    };

    struct call
        : public expr_base
    {
        call(util::sref<function const> f, std::vector<util::sptr<expr_base const>> a)
            : func(f)
            , args(std::move(a))
        {}

        type const* typeof() const;
        void write() const;

        util::sref<function const> const func;
        std::vector<util::sptr<expr_base const>> args;
    };

    struct binary_op
        : public expr_base
    {
        binary_op(util::sptr<expr_base const> l, operation const* o, util::sptr<expr_base const> r)
            : lhs(std::move(l))
            , op(o)
            , rhs(std::move(r))
        {}

        type const* typeof() const;
        void write() const;

        util::sptr<expr_base const> const lhs;
        operation const* const op;
        util::sptr<expr_base const> const rhs;
    };

    struct pre_unary_op
        : public expr_base
    {
        pre_unary_op(operation const* o, util::sptr<expr_base const> r)
            : op(o)
            , rhs(std::move(r))
        {}

        type const* typeof() const;
        void write() const;

        operation const* const op;
        util::sptr<expr_base const> const rhs;
    };

    struct conjunction
        : public expr_base
    {
        conjunction(misc::pos_type const& p, util::sptr<expr_base const> l, util::sptr<expr_base const> r);

        type const* typeof() const;
        void write() const;

        util::sptr<expr_base const> const lhs;
        util::sptr<expr_base const> const rhs;
    };

    struct disjunction
        : public expr_base
    {
        disjunction(misc::pos_type const& p, util::sptr<expr_base const> l, util::sptr<expr_base const> r);

        type const* typeof() const;
        void write() const;

        util::sptr<expr_base const> const lhs;
        util::sptr<expr_base const> const rhs;
    };

    struct negation
        : public expr_base
    {
        explicit negation(misc::pos_type const& p, util::sptr<expr_base const> r);

        type const* typeof() const;
        void write() const;

        util::sptr<expr_base const> const rhs;
    };

}

#endif /* __STACKENING_INSTANCE_EXPRESSION_NODES_H__ */
