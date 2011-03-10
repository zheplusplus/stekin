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

        void compile(util::sref<proto::scope> scope) const;

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

        void compile(util::sref<proto::scope> scope) const;

        util::sptr<expr_base const> const predicate;
        block const consequence;
        block const alternative;
    };

    struct func_ret
        : public stmt_base
    {
        func_ret(misc::pos_type const& pos, util::sptr<expr_base const> retval)
            : stmt_base(pos)
            , ret_val(std::move(retval))
        {}

        void compile(util::sref<proto::scope> scope) const;

        util::sptr<expr_base const> const ret_val;
    };

    struct func_ret_nothing
        : public stmt_base
    {
        explicit func_ret_nothing(misc::pos_type const& pos)
            : stmt_base(pos)
        {}

        void compile(util::sref<proto::scope> scope) const;
    };

    struct var_def
        : public stmt_base
    {
        var_def(misc::pos_type const& pos, std::string const& var_name, util::sptr<expr_base const> var_init)
            : stmt_base(pos)
            , name(var_name)
            , init(std::move(var_init))
        {}

        void compile(util::sref<proto::scope> scope) const;

        std::string const name;
        util::sptr<expr_base const> const init;
    };

}

#endif /* __STAKCENING_GRAMMAR_STATEMENT_NODES_H__ */
