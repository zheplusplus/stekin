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
        arithmetics(misc::position const& pos, util::sptr<Expression const> e)
            : Statement(pos)
            , expr(std::move(e))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        util::sptr<Expression const> const expr;
    };

    struct branch
        : public Statement
    {
        branch(misc::position const& pos, util::sptr<Expression const> p, Block c, Block a)
            : Statement(pos)
            , predicate(std::move(p))
            , consequence(std::move(c))
            , alternative(std::move(a))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        util::sptr<Expression const> const predicate;
        Block const consequence;
        Block const alternative;
    };

    struct branch_cons_only
        : public Statement
    {
        branch_cons_only(misc::position const& pos, util::sptr<Expression const> p, Block c)
            : Statement(pos)
            , predicate(std::move(p))
            , consequence(std::move(c))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        util::sptr<Expression const> const predicate;
        Block const consequence;
    };

    struct branch_alt_only
        : public Statement
    {
        branch_alt_only(misc::position const& pos, util::sptr<Expression const> p, Block a)
            : Statement(pos)
            , predicate(std::move(p))
            , alternative(std::move(a))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        util::sptr<Expression const> const predicate;
        Block const alternative;
    };

    struct func_ret
        : public Statement
    {
        func_ret(misc::position const& pos, util::sptr<Expression const> r)
            : Statement(pos)
            , ret_val(std::move(r))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        util::sptr<Expression const> const ret_val;
    };

    struct func_ret_nothing
        : public Statement
    {
        explicit func_ret_nothing(misc::position const& pos)
            : Statement(pos)
        {}

        void compile(util::sref<flchk::filter> filter) const;
    };

    struct var_def
        : public Statement
    {
        var_def(misc::position const& pos, std::string const& n, util::sptr<Expression const> i)
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
