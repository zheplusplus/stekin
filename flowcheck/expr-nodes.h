#ifndef __STAKCENING_FLOW_CHECK_EXPRESSION_NODES_H__
#define __STAKCENING_FLOW_CHECK_EXPRESSION_NODES_H__

#include <string>
#include <vector>

#include "node-base.h"
#include "../util/pointer.h"

namespace flchk {

    struct pre_unary_op
        : public expr_base
    {
        pre_unary_op(misc::pos_type const& pos, std::string const& op, util::sptr<expr_base const> r)
            : expr_base(pos)
            , op_img(op)
            , rhs(std::move(r))
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        util::sptr<expr_base const> fold() const;
        std::string type_name() const;

        std::string const op_img;
        util::sptr<expr_base const> const rhs;
    };

    struct binary_op
        : public expr_base
    {
        binary_op(misc::pos_type const& pos
                , util::sptr<expr_base const> l
                , std::string const& op
                , util::sptr<expr_base const> r)
            : expr_base(pos)
            , lhs(std::move(l))
            , op_img(op)
            , rhs(std::move(r))
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        std::string type_name() const;
        util::sptr<expr_base const> fold() const;

        util::sptr<expr_base const> const lhs;
        std::string const op_img;
        util::sptr<expr_base const> const rhs;
    };

    struct conjunction
        : public expr_base
    {
        conjunction(misc::pos_type const& pos, util::sptr<expr_base const> l, util::sptr<expr_base const> r)
            : expr_base(pos)
            , lhs(std::move(l))
            , rhs(std::move(r))
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        std::string type_name() const;
        util::sptr<expr_base const> fold() const;

        util::sptr<expr_base const> const lhs;
        util::sptr<expr_base const> const rhs;
    };

    struct disjunction
        : public expr_base
    {
        disjunction(misc::pos_type const& pos, util::sptr<expr_base const> l, util::sptr<expr_base const> r)
            : expr_base(pos)
            , lhs(std::move(l))
            , rhs(std::move(r))
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        std::string type_name() const;
        util::sptr<expr_base const> fold() const;

        util::sptr<expr_base const> const lhs;
        util::sptr<expr_base const> const rhs;
    };

    struct negation
        : public expr_base
    {
        negation(misc::pos_type const& pos, util::sptr<expr_base const> r)
            : expr_base(pos)
            , rhs(std::move(r))
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        std::string type_name() const;
        util::sptr<expr_base const> fold() const;

        util::sptr<expr_base const> const rhs;
    };

    struct reference
        : public expr_base
    {
        reference(misc::pos_type const& pos, std::string const& n)
            : expr_base(pos)
            , name(n)
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;
        std::string type_name() const;
        util::sptr<expr_base const> fold() const;

        std::string const name;
    };

    struct bool_literal
        : public expr_base
    {
        bool_literal(misc::pos_type const& pos, bool v)
            : expr_base(pos)
            , value(v)
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        std::string type_name() const;
        util::sptr<expr_base const> fold() const;

        util::sptr<expr_base const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , mpz_class const&) const;
        util::sptr<expr_base const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , mpf_class const&) const;
        util::sptr<expr_base const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , bool rhs) const;

        util::sptr<expr_base const> as_rhs(misc::pos_type const& op_pos
                                         , std::string const& op_img
                                         , util::sptr<expr_base const> lhs) const;
        util::sptr<expr_base const> as_rhs(misc::pos_type const& op_pos, std::string const& op_img) const;

        bool const value;
    public:
        static util::sptr<expr_base const> mkbool(bool value);
    };

    struct int_literal
        : public expr_base
    {
        int_literal(misc::pos_type const& pos, std::string const& image)
            : expr_base(pos)
            , value(image)
        {}

        int_literal(misc::pos_type const& pos, mpz_class const& v)
            : expr_base(pos)
            , value(v)
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        std::string type_name() const;
        util::sptr<expr_base const> fold() const;

        util::sptr<expr_base const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , mpz_class const& rhs) const;
        util::sptr<expr_base const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , mpf_class const& rhs) const;
        util::sptr<expr_base const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , bool) const;

        util::sptr<expr_base const> as_rhs(misc::pos_type const& op_pos
                                         , std::string const& op_img
                                         , util::sptr<expr_base const> lhs) const;
        util::sptr<expr_base const> as_rhs(misc::pos_type const& op_pos, std::string const& op_img) const;

        mpz_class const value;
    };

    struct float_literal
        : public expr_base
    {
        float_literal(misc::pos_type const& pos, std::string const& image)
            : expr_base(pos)
            , value(image)
        {}

        float_literal(misc::pos_type const& pos, mpf_class const& v)
            : expr_base(pos)
            , value(v)
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        std::string type_name() const;
        util::sptr<expr_base const> fold() const;

        util::sptr<expr_base const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , mpz_class const& rhs) const;
        util::sptr<expr_base const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , mpf_class const& rhs) const;
        util::sptr<expr_base const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , bool) const;

        util::sptr<expr_base const> as_rhs(misc::pos_type const& op_pos
                                         , std::string const& op_img
                                         , util::sptr<expr_base const> lhs) const;
        util::sptr<expr_base const> as_rhs(misc::pos_type const& op_pos, std::string const& op_img) const;

        mpf_class const value;
    };

    struct call
        : public expr_base
    {
        call(misc::pos_type const& pos, std::string const& n, std::vector<util::sptr<expr_base const>> a)
            : expr_base(pos)
            , name(n)
            , args(std::move(a))
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;
        std::string type_name() const;
        util::sptr<expr_base const> fold() const;

        std::string const name;
        std::vector<util::sptr<expr_base const>> const args;
    };

    struct func_reference
        : public expr_base
    {
        func_reference(misc::pos_type const& pos, std::string const& n, int pc)
            : expr_base(pos)
            , name(n)
            , param_count(pc)
        {}

        util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const;
        std::string type_name() const;
        util::sptr<expr_base const> fold() const;

        std::string const name;
        int const param_count;
    };

}

#endif /* _STAKCENING_FLOW_CHECK_EXPRESSION_NODES_H__ */
