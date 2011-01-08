#ifndef __STACKENING_INSTANCE_STATEMENT_NODES_H__
#define __STACKENING_INSTANCE_STATEMENT_NODES_H__

#include "node-base.h"
#include "expr-nodes.h"
#include "../util/pointer.h"

namespace inst {

    struct arithmetics
        : public stmt_base
    {
        explicit arithmetics(util::sptr<expr_base const>&& e)
            : expr(std::move(e))
        {}

        util::sptr<expr_base const> const expr;
    };

    struct branch
        : public stmt_base
    {
        branch(util::sptr<expr_base const>&& c
             , util::sptr<stmt_base const>&& v
             , util::sptr<stmt_base const>&& i)
                : condition(std::move(c))
                , valid(std::move(v))
                , invalid(std::move(i))
        {}

        util::sptr<expr_base const> const condition;
        util::sptr<stmt_base const> const valid;
        util::sptr<stmt_base const> const invalid;
    };

    struct loop
        : public stmt_base
    {
        loop(util::sptr<expr_base const>&& c, util::sptr<stmt_base const>&& b)
            : condition(std::move(c))
            , body(std::move(b))
        {}

        util::sptr<expr_base const> const condition;
        util::sptr<stmt_base const> const body;
    };

    struct initialization
        : public stmt_base
    {
        initialization(int o, util::sptr<expr_base const>&& i)
            : offset(o)
            , init(std::move(i))
        {}

        int const offset;
        util::sptr<expr_base const> const init;
    };

    struct func_ret
        : public stmt_base
    {
        explicit func_ret(util::sptr<expr_base const> r)
            : ret_val(std::move(r))
        {}

        util::sptr<expr_base const> const ret_val;
    };

    struct func_ret_nothing
        : public stmt_base
    {};

}

#endif /* __STACKENING_INSTANCE_STATEMENT_NODES_H__ */
