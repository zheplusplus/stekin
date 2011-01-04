#ifndef _STAKCENING_GRAMMAR_EXPRESSION_NODES_H__
#define _STAKCENING_GRAMMAR_EXPRESSION_NODES_H__

#include <string>

#include "node-base.h"
#include "../util/pointer.h"

namespace grammar {

    struct pre_unary_oper
        : public expr_base
    {
        pre_unary_oper(misc::pos_type const& pos, std::string const& op, expr_base const* r)
            : expr_base(pos)
            , op_img(op)
            , rhs(r)
        {}

        proto::expr_prototype const* compile(proto::scope const* scope) const;

        std::string const op_img;
        util::sptr<expr_base const> const rhs;
    };

    struct binary_oper
        : public expr_base
    {
        binary_oper(misc::pos_type const& pos, expr_base const* l, std::string const& op, expr_base const* r)
            : expr_base(pos)
            , lhs(l)
            , op_img(op)
            , rhs(r)
        {}

        proto::expr_prototype const* compile(proto::scope const* scope) const;

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

        proto::expr_prototype const* compile(proto::scope const* scope) const;

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

        proto::expr_prototype const* compile(proto::scope const* scope) const;

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

        proto::expr_prototype const* compile(proto::scope const* scope) const;

        util::sptr<expr_base const> const rhs;
    };

    struct reference
        : public expr_base
    {
        reference(misc::pos_type const& pos, std::string const& name)
            : expr_base(pos)
            , name(name)
        {}

        proto::expr_prototype const* compile(proto::scope const* scope) const;

        std::string const name;
    };

    struct int_literal
        : public expr_base
    {
        int_literal(misc::pos_type const& pos, char const* val)
            : expr_base(pos)
            , value(val)
        {}

        proto::expr_prototype const* compile(proto::scope const* scope) const;

        std::string const value;
    };

    struct float_literal
        : public expr_base
    {
        float_literal(misc::pos_type const& pos, char const* val)
            : expr_base(pos)
            , value(val)
        {}

        proto::expr_prototype const* compile(proto::scope const* scope) const;

        std::string const value;
    };

    struct call
        : public expr_base
    {
        template <typename ArgIterator>
        call(misc::pos_type const& pos, std::string const& name, ArgIterator arg_begin, ArgIterator arg_end)
            : expr_base(pos)
            , name(name)
            , args(arg_begin, arg_end)
        {}

        proto::expr_prototype const* compile(proto::scope const* scope) const;

        std::string const name;
        std::vector<util::sptr<expr_base const>> const args;
    };

}

#endif /* _STAKCENING_GRAMMAR_EXPRESSION_NODES_H__ */
