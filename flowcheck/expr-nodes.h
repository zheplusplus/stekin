#ifndef __STAKCENING_FLOW_CHECK_EXPRESSION_NODES_H__
#define __STAKCENING_FLOW_CHECK_EXPRESSION_NODES_H__

#include <string>
#include <vector>

#include "node-base.h"
#include "../util/pointer.h"

namespace flchk {

    struct pre_unary_op
        : public Expression
    {
        pre_unary_op(misc::pos_type const& pos, std::string const& op, util::sptr<Expression const> r)
            : Expression(pos)
            , op_img(op)
            , rhs(std::move(r))
        {}

        util::sptr<proto::Expression const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        util::sptr<Expression const> fold() const;
        std::string type_name() const;

        std::string const op_img;
        util::sptr<Expression const> const rhs;
    };

    struct binary_op
        : public Expression
    {
        binary_op(misc::pos_type const& pos
                , util::sptr<Expression const> l
                , std::string const& op
                , util::sptr<Expression const> r)
            : Expression(pos)
            , lhs(std::move(l))
            , op_img(op)
            , rhs(std::move(r))
        {}

        util::sptr<proto::Expression const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        std::string type_name() const;
        util::sptr<Expression const> fold() const;

        util::sptr<Expression const> const lhs;
        std::string const op_img;
        util::sptr<Expression const> const rhs;
    };

    struct conjunction
        : public Expression
    {
        conjunction(misc::pos_type const& pos, util::sptr<Expression const> l, util::sptr<Expression const> r)
            : Expression(pos)
            , lhs(std::move(l))
            , rhs(std::move(r))
        {}

        util::sptr<proto::Expression const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        std::string type_name() const;
        util::sptr<Expression const> fold() const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct disjunction
        : public Expression
    {
        disjunction(misc::pos_type const& pos, util::sptr<Expression const> l, util::sptr<Expression const> r)
            : Expression(pos)
            , lhs(std::move(l))
            , rhs(std::move(r))
        {}

        util::sptr<proto::Expression const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        std::string type_name() const;
        util::sptr<Expression const> fold() const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct negation
        : public Expression
    {
        negation(misc::pos_type const& pos, util::sptr<Expression const> r)
            : Expression(pos)
            , rhs(std::move(r))
        {}

        util::sptr<proto::Expression const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        std::string type_name() const;
        util::sptr<Expression const> fold() const;

        util::sptr<Expression const> const rhs;
    };

    struct reference
        : public Expression
    {
        reference(misc::pos_type const& pos, std::string const& n)
            : Expression(pos)
            , name(n)
        {}

        util::sptr<proto::Expression const> compile(util::sref<proto::scope> scope) const;
        std::string type_name() const;
        util::sptr<Expression const> fold() const;

        std::string const name;
    };

    struct BoolLiteral
        : public Expression
    {
        BoolLiteral(misc::pos_type const& pos, bool v)
            : Expression(pos)
            , value(v)
        {}

        util::sptr<proto::Expression const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        std::string type_name() const;
        util::sptr<Expression const> fold() const;

        util::sptr<Expression const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , mpz_class const&) const;
        util::sptr<Expression const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , mpf_class const&) const;
        util::sptr<Expression const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , bool rhs) const;

        util::sptr<Expression const> as_rhs(misc::pos_type const& op_pos
                                         , std::string const& op_img
                                         , util::sptr<Expression const> lhs) const;
        util::sptr<Expression const> as_rhs(misc::pos_type const& op_pos, std::string const& op_img) const;

        bool const value;
    public:
        static util::sptr<Expression const> mkbool(bool value);
    };

    struct IntLiteral
        : public Expression
    {
        IntLiteral(misc::pos_type const& pos, std::string const& image)
            : Expression(pos)
            , value(image)
        {}

        IntLiteral(misc::pos_type const& pos, mpz_class const& v)
            : Expression(pos)
            , value(v)
        {}

        util::sptr<proto::Expression const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        std::string type_name() const;
        util::sptr<Expression const> fold() const;

        util::sptr<Expression const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , mpz_class const& rhs) const;
        util::sptr<Expression const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , mpf_class const& rhs) const;
        util::sptr<Expression const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , bool) const;

        util::sptr<Expression const> as_rhs(misc::pos_type const& op_pos
                                         , std::string const& op_img
                                         , util::sptr<Expression const> lhs) const;
        util::sptr<Expression const> as_rhs(misc::pos_type const& op_pos, std::string const& op_img) const;

        mpz_class const value;
    };

    struct FloatLiteral
        : public Expression
    {
        FloatLiteral(misc::pos_type const& pos, std::string const& image)
            : Expression(pos)
            , value(image)
        {}

        FloatLiteral(misc::pos_type const& pos, mpf_class const& v)
            : Expression(pos)
            , value(v)
        {}

        util::sptr<proto::Expression const> compile(util::sref<proto::scope> scope) const;
        bool is_literal() const;
        bool bool_value() const;
        std::string type_name() const;
        util::sptr<Expression const> fold() const;

        util::sptr<Expression const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , mpz_class const& rhs) const;
        util::sptr<Expression const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , mpf_class const& rhs) const;
        util::sptr<Expression const> operate(misc::pos_type const& op_pos
                                          , std::string const& op_img
                                          , bool) const;

        util::sptr<Expression const> as_rhs(misc::pos_type const& op_pos
                                         , std::string const& op_img
                                         , util::sptr<Expression const> lhs) const;
        util::sptr<Expression const> as_rhs(misc::pos_type const& op_pos, std::string const& op_img) const;

        mpf_class const value;
    };

    struct call
        : public Expression
    {
        call(misc::pos_type const& pos, std::string const& n, std::vector<util::sptr<Expression const>> a)
            : Expression(pos)
            , name(n)
            , args(std::move(a))
        {}

        util::sptr<proto::Expression const> compile(util::sref<proto::scope> scope) const;
        std::string type_name() const;
        util::sptr<Expression const> fold() const;

        std::string const name;
        std::vector<util::sptr<Expression const>> const args;
    };

    struct func_reference
        : public Expression
    {
        func_reference(misc::pos_type const& pos, std::string const& n, int pc)
            : Expression(pos)
            , name(n)
            , param_count(pc)
        {}

        util::sptr<proto::Expression const> compile(util::sref<proto::scope> scope) const;
        std::string type_name() const;
        util::sptr<Expression const> fold() const;

        std::string const name;
        int const param_count;
    };

}

#endif /* _STAKCENING_FLOW_CHECK_EXPRESSION_NODES_H__ */
