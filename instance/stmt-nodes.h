#ifndef __STACKENING_INSTANCE_STATEMENT_NODES_H__
#define __STACKENING_INSTANCE_STATEMENT_NODES_H__

#include "node-base.h"
#include "expr-nodes.h"
#include "../util/pointer.h"

namespace inst {

    struct arithmetics
        : public stmt_base
    {
        explicit arithmetics(expr_base const* e)
            : expr(e)
        {}

        util::sptr<expr_base const> const expr;
    };

    struct branch
        : public stmt_base
    {
        branch(expr_base const* c, stmt_base const* v, stmt_base const* i)
            : condition(c)
            , valid(v)
            , invalid(i)
        {}

        util::sptr<expr_base const> const condition;
        util::sptr<stmt_base const> const valid;
        util::sptr<stmt_base const> const invalid;
    };

    struct loop
        : public stmt_base
    {
        loop(expr_base const* c, stmt_base const* b)
            : condition(c)
            , body(b)
        {}

        util::sptr<expr_base const> const condition;
        util::sptr<stmt_base const> const body;
    };

    struct initialization
        : public stmt_base
    {
        initialization(int o, expr_base const* i)
            : offset(o)
            , init(i)
        {}

        int const offset;
        util::sptr<expr_base const> const init;
    };

    struct func_ret
        : public stmt_base
    {
        explicit func_ret(expr_base const* r)
            : ret_val(r)
        {}

        util::sptr<expr_base const> const ret_val;
    };

    struct func_ret_nothing
        : public stmt_base
    {};

}

#endif /* __STACKENING_INSTANCE_STATEMENT_NODES_H__ */
