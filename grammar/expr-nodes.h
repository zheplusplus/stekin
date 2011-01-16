#ifndef _STAKCENING_GRAMMAR_EXPRESSION_NODES_H__
#define _STAKCENING_GRAMMAR_EXPRESSION_NODES_H__

#include <string>

#include "node-base.h"
#include "../util/pointer.h"

namespace grammar {

    struct pre_unary_op
        : public expr_base
    {
        pre_unary_op(misc::pos_type const& pos, std::string const& op, expr_base const* r)
            : expr_base(pos)
            , op_img(op)
            , rhs(r)
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;

        std::string const op_img;
        util::sptr<expr_base const> const rhs;
    };

    struct binary_op
        : public expr_base
    {
        binary_op(misc::pos_type const& pos, expr_base const* l, std::string const& op, expr_base const* r)
            : expr_base(pos)
            , lhs(l)
            , op_img(op)
            , rhs(r)
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;

        util::sptr<expr_base const> const lhs;
        std::string const op_img;
        util::sptr<expr_base const> const rhs;
    };

    struct conjunction
        : public expr_base
    {
        conjunction(misc::pos_type const& pos, expr_base const* l, expr_base const* r)
            : expr_base(pos)
            , lhs(l)
            , rhs(r)
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;

        util::sptr<expr_base const> const lhs;
        util::sptr<expr_base const> const rhs;
    };

    struct disjunction
        : public expr_base
    {
        disjunction(misc::pos_type const& pos, expr_base const* l, expr_base const* r)
            : expr_base(pos)
            , lhs(l)
            , rhs(r)
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;

        util::sptr<expr_base const> const lhs;
        util::sptr<expr_base const> const rhs;
    };

    struct negation
        : public expr_base
    {
        negation(misc::pos_type const& pos, expr_base const* r)
            : expr_base(pos)
            , rhs(r)
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;

        util::sptr<expr_base const> const rhs;
    };

    struct reference
        : public expr_base
    {
        reference(misc::pos_type const& pos, std::string const& name)
            : expr_base(pos)
            , name(name)
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;

        std::string const name;
    };

    struct bool_literal
        : public expr_base
    {
        bool_literal(misc::pos_type const& pos, bool val)
            : expr_base(pos)
            , value(val)
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;

        bool const value;
    };

    struct int_literal
        : public expr_base
    {
        int_literal(misc::pos_type const& pos, char const* val)
            : expr_base(pos)
            , value(val)
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;

        std::string const value;
    };

    struct float_literal
        : public expr_base
    {
        float_literal(misc::pos_type const& pos, char const* val)
            : expr_base(pos)
            , value(val)
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;

        std::string const value;
    };

    struct call
        : public expr_base
    {
        call(misc::pos_type const& pos, std::string const& name, std::list<util::sptr<expr_base const>> a)
            : expr_base(pos)
            , name(name)
            , args(std::move(a))
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;

        std::string const name;
        std::list<util::sptr<expr_base const>> const args;
    };

}

#endif /* _STAKCENING_GRAMMAR_EXPRESSION_NODES_H__ */
