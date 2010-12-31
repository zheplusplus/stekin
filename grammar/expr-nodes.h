#ifndef __STAKCENING_GRAMMAR_EXPRESSION_NODES_H__
#define __STAKCENING_GRAMMAR_EXPRESSION_NODES_H__

#include <string>

#include "node-base.h"
#include "../util/pointer.h"

namespace grammar {

    struct pre_unary_oper
        : public expr_base
    {
        pre_unary_oper(misc::pos_type const& pos, std::string const& op_img, expr_base const* rhs)
            : expr_base(pos)
            , _op_img(op_img)
            , _rhs(rhs)
        {}

        proto::expr_prototype const* compile(proto::prototype_scope const* scope) const;
    protected:
        std::string const _op_img;
        util::sptr<expr_base const> const _rhs;
    };

    struct binary_oper
        : public expr_base
    {
        binary_oper(misc::pos_type const& pos, expr_base const* lhs, std::string const& op_img, expr_base const* rhs)
            : expr_base(pos)
            , _lhs(lhs)
            , _op_img(op_img)
            , _rhs(rhs)
        {}

        proto::expr_prototype const* compile(proto::prototype_scope const* scope) const;
    protected:
        util::sptr<expr_base const> const _lhs;
        std::string const _op_img;
        util::sptr<expr_base const> const _rhs;
    };

    struct conjunction
        : public expr_base
    {
        conjunction(misc::pos_type const& pos, expr_base const* lhs, expr_base const* rhs)
            : expr_base(pos)
            , _lhs(lhs)
            , _rhs(rhs)
        {}

        proto::expr_prototype const* compile(proto::prototype_scope const* scope) const;
    protected:
        util::sptr<expr_base const> const _lhs;
        util::sptr<expr_base const> const _rhs;
    };

    struct disjunction
        : public expr_base
    {
        disjunction(misc::pos_type const& pos, expr_base const* lhs, expr_base const* rhs)
            : expr_base(pos)
            , _lhs(lhs)
            , _rhs(rhs)
        {}

        proto::expr_prototype const* compile(proto::prototype_scope const* scope) const;
    protected:
        util::sptr<expr_base const> const _lhs;
        util::sptr<expr_base const> const _rhs;
    };

    struct negation
        : public expr_base
    {
        negation(misc::pos_type const& pos, expr_base const* rhs)
            : expr_base(pos)
            , _rhs(rhs)
        {}

        proto::expr_prototype const* compile(proto::prototype_scope const* scope) const;
    protected:
        util::sptr<expr_base const> const _rhs;
    };

    struct variable
        : public expr_base
    {
        variable(misc::pos_type const& pos, std::string const& name)
            : expr_base(pos)
            , _name(name)
        {}

        proto::expr_prototype const* compile(proto::prototype_scope const* scope) const;
    protected:
        std::string const _name;
    };

    struct int_literal
        : public expr_base
    {
        int_literal(misc::pos_type const& pos, char const* val)
            : expr_base(pos)
            , _val(val)
        {}

        proto::expr_prototype const* compile(proto::prototype_scope const* scope) const;
    protected:
        std::string const _val;
    };

    struct float_literal
        : public expr_base
    {
        float_literal(misc::pos_type const& pos, char const* val)
            : expr_base(pos)
            , _val(val)
        {}

        proto::expr_prototype const* compile(proto::prototype_scope const* scope) const;
    protected:
        std::string const _val;
    };

    struct func_call
        : public expr_base
    {
        template <typename _ArgIterator>
        func_call(misc::pos_type const& pos
                , std::string const& name
                , _ArgIterator arg_begin
                , _ArgIterator arg_end)
            : expr_base(pos)
            , _name(name)
            , _args(arg_begin, arg_end)
        {}

        proto::expr_prototype const* compile(proto::prototype_scope const* scope) const;
    protected:
        std::string const _name;
        std::vector<util::sptr<expr_base const>> const _args;
    };

}

#endif /* __STAKCENING_GRAMMAR_EXPRESSION_NODES_H__ */
