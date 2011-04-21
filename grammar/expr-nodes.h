#ifndef __STAKCENING_GRAMMAR_EXPRESSION_NODES_H__
#define __STAKCENING_GRAMMAR_EXPRESSION_NODES_H__

#include <vector>

#include "node-base.h"

namespace grammar {

    struct pre_unary_op
        : public Expression
    {
        pre_unary_op(misc::pos_type const& pos, std::string const& op, util::sptr<Expression const> r)
            : Expression(pos)
            , op_img(op)
            , rhs(std::move(r))
        {}

        util::sptr<flchk::Expression const> compile() const;

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

        util::sptr<flchk::Expression const> compile() const;

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

        util::sptr<flchk::Expression const> compile() const;

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

        util::sptr<flchk::Expression const> compile() const;

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

        util::sptr<flchk::Expression const> compile() const;

        util::sptr<Expression const> const rhs;
    };

    struct reference
        : public Expression
    {
        reference(misc::pos_type const& pos, std::string const& n)
            : Expression(pos)
            , name(n)
        {}

        util::sptr<flchk::Expression const> compile() const;

        std::string const name;
    };

    struct bool_literal
        : public Expression
    {
        bool_literal(misc::pos_type const& pos, bool val)
            : Expression(pos)
            , value(val)
        {}

        util::sptr<flchk::Expression const> compile() const;

        bool const value;
    };

    struct IntLiteral
        : public Expression
    {
        IntLiteral(misc::pos_type const& pos, std::string const& val)
            : Expression(pos)
            , value(val)
        {}

        util::sptr<flchk::Expression const> compile() const;

        std::string const value;
    };

    struct float_literal
        : public Expression
    {
        float_literal(misc::pos_type const& pos, std::string const& val)
            : Expression(pos)
            , value(val)
        {}

        util::sptr<flchk::Expression const> compile() const;

        std::string const value;
    };

    struct call
        : public Expression
    {
        call(misc::pos_type const& pos, std::string const& n, std::vector<util::sptr<Expression const>> a)
            : Expression(pos)
            , name(n)
            , args(std::move(a))
        {}

        util::sptr<flchk::Expression const> compile() const;

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

        util::sptr<flchk::Expression const> compile() const;

        std::string const name;
        int const param_count;
    };

}

#endif /* _STAKCENING_GRAMMAR_EXPRESSION_NODES_H__ */
