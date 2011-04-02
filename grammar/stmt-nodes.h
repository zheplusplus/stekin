#ifndef __STAKCENING_GRAMMAR_STATEMENT_NODES_H__
#define __STAKCENING_GRAMMAR_STATEMENT_NODES_H__

#include <string>
#include <list>

#include "node-base.h"
#include "block.h"
#include "../util/pointer.h"

namespace grammar {

    struct arithmetics
        : public stmt_base
    {
        arithmetics(misc::pos_type const& pos, util::sptr<expr_base const> e)
            : stmt_base(pos)
            , expr(std::move(e))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        util::sptr<expr_base const> const expr;
    };

    struct branch
        : public stmt_base
    {
        branch(misc::pos_type const& pos, util::sptr<expr_base const> p, block c, block a)
            : stmt_base(pos)
            , predicate(std::move(p))
            , consequence(std::move(c))
            , alternative(std::move(a))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        util::sptr<expr_base const> const predicate;
        block const consequence;
        block const alternative;
    };

    struct branch_cons_only
        : public stmt_base
    {
        branch_cons_only(misc::pos_type const& pos, util::sptr<expr_base const> p, block c)
            : stmt_base(pos)
            , predicate(std::move(p))
            , consequence(std::move(c))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        util::sptr<expr_base const> const predicate;
        block const consequence;
    };

    struct branch_alt_only
        : public stmt_base
    {
        branch_alt_only(misc::pos_type const& pos, util::sptr<expr_base const> p, block a)
            : stmt_base(pos)
            , predicate(std::move(p))
            , alternative(std::move(a))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        util::sptr<expr_base const> const predicate;
        block const alternative;
    };

    struct func_ret
        : public stmt_base
    {
        func_ret(misc::pos_type const& pos, util::sptr<expr_base const> r)
            : stmt_base(pos)
            , ret_val(std::move(r))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        util::sptr<expr_base const> const ret_val;
    };

    struct func_ret_nothing
        : public stmt_base
    {
        explicit func_ret_nothing(misc::pos_type const& pos)
            : stmt_base(pos)
        {}

        void compile(util::sref<flchk::filter> filter) const;
    };

    struct var_def
        : public stmt_base
    {
        var_def(misc::pos_type const& pos, std::string const& n, util::sptr<expr_base const> i)
            : stmt_base(pos)
            , name(n)
            , init(std::move(i))
        {}

        void compile(util::sref<flchk::filter> filter) const;

        std::string const name;
        util::sptr<expr_base const> const init;
    };

}

#endif /* __STAKCENING_GRAMMAR_STATEMENT_NODES_H__ */
