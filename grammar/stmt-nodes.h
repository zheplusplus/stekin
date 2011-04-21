#ifndef __STAKCENING_GRAMMAR_STATEMENT_NODES_H__
#define __STAKCENING_GRAMMAR_STATEMENT_NODES_H__

#include <string>
#include <list>

#include "node-base.h"
#include "block.h"
#include "../util/pointer.h"

namespace grammar {

    struct arithmetics
        : public Statement
    {
        arithmetics(misc::pos_type const& pos, util::sptr<Expression const> e)
            : Statement(pos)
            , expr(std::move(e))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        util::sptr<Expression const> const expr;
    };

    struct branch
        : public Statement
    {
        branch(misc::pos_type const& pos, util::sptr<Expression const> p, block c, block a)
            : Statement(pos)
            , predicate(std::move(p))
            , consequence(std::move(c))
            , alternative(std::move(a))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        util::sptr<Expression const> const predicate;
        block const consequence;
        block const alternative;
    };

    struct branch_cons_only
        : public Statement
    {
        branch_cons_only(misc::pos_type const& pos, util::sptr<Expression const> p, block c)
            : Statement(pos)
            , predicate(std::move(p))
            , consequence(std::move(c))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        util::sptr<Expression const> const predicate;
        block const consequence;
    };

    struct branch_alt_only
        : public Statement
    {
        branch_alt_only(misc::pos_type const& pos, util::sptr<Expression const> p, block a)
            : Statement(pos)
            , predicate(std::move(p))
            , alternative(std::move(a))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        util::sptr<Expression const> const predicate;
        block const alternative;
    };

    struct func_ret
        : public Statement
    {
        func_ret(misc::pos_type const& pos, util::sptr<Expression const> r)
            : Statement(pos)
            , ret_val(std::move(r))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        util::sptr<Expression const> const ret_val;
    };

    struct func_ret_nothing
        : public Statement
    {
        explicit func_ret_nothing(misc::pos_type const& pos)
            : Statement(pos)
        {}

        void compile(util::sref<flchk::filter> filter) const;
    };

    struct var_def
        : public Statement
    {
        var_def(misc::pos_type const& pos, std::string const& n, util::sptr<Expression const> i)
            : Statement(pos)
            , name(n)
            , init(std::move(i))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        std::string const name;
        util::sptr<Expression const> const init;
    };

}

#endif /* __STAKCENING_GRAMMAR_STATEMENT_NODES_H__ */
