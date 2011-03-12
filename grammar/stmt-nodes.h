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
            , _expr(std::move(e))
        {}

        void compile(util::sref<flchk::filter> filter);
    private:
        util::sptr<expr_base const> _expr;
    };

    struct branch
        : public stmt_base
    {
        branch(misc::pos_type const& pos, util::sptr<expr_base const> p, block c, block a)
            : stmt_base(pos)
            , _predicate(std::move(p))
            , _consequence(std::move(c))
            , _alternative(std::move(a))
        {}

        void compile(util::sref<flchk::filter> filter);
    private:
        util::sptr<expr_base const> _predicate;
        block _consequence;
        block _alternative;
    };

    struct branch_cons_only
        : public stmt_base
    {
        branch_cons_only(misc::pos_type const& pos, util::sptr<expr_base const> p, block c)
            : stmt_base(pos)
            , _predicate(std::move(p))
            , _consequence(std::move(c))
        {}

        void compile(util::sref<flchk::filter> filter);
    private:
        util::sptr<expr_base const> _predicate;
        block _consequence;
    };

    struct branch_alt_only
        : public stmt_base
    {
        branch_alt_only(misc::pos_type const& pos, util::sptr<expr_base const> p, block a)
            : stmt_base(pos)
            , _predicate(std::move(p))
            , _alternative(std::move(a))
        {}

        void compile(util::sref<flchk::filter> filter);
    private:
        util::sptr<expr_base const> _predicate;
        block _alternative;
    };

    struct func_ret
        : public stmt_base
    {
        func_ret(misc::pos_type const& pos, util::sptr<expr_base const> retval)
            : stmt_base(pos)
            , _ret_val(std::move(retval))
        {}

        void compile(util::sref<flchk::filter> filter);
    private:
        util::sptr<expr_base const> _ret_val;
    };

    struct func_ret_nothing
        : public stmt_base
    {
        explicit func_ret_nothing(misc::pos_type const& pos)
            : stmt_base(pos)
        {}

        void compile(util::sref<flchk::filter> filter);
    };

    struct var_def
        : public stmt_base
    {
        var_def(misc::pos_type const& pos, std::string const& var_name, util::sptr<expr_base const> var_init)
            : stmt_base(pos)
            , _name(var_name)
            , _init(std::move(var_init))
        {}

        void compile(util::sref<flchk::filter> filter);
    private:
        std::string const _name;
        util::sptr<expr_base const> _init;
    };

}

#endif /* __STAKCENING_GRAMMAR_STATEMENT_NODES_H__ */
