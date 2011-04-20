#ifndef __STEKIN_INSTANCE_STATEMENT_NODES_H__
#define __STEKIN_INSTANCE_STATEMENT_NODES_H__

#include "node-base.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace inst {

    struct arithmetics
        : public stmt_base
    {
        explicit arithmetics(util::sptr<expr_base const> e)
            : expr(std::move(e))
        {}

        void write() const;

        util::sptr<expr_base const> const expr;
    };

    struct branch
        : public stmt_base
    {
        branch(misc::pos_type const& pos
             , util::sptr<expr_base const> p
             , util::sptr<stmt_base const> c
             , util::sptr<stmt_base const> a);

        void write() const;

        util::sptr<expr_base const> const predicate;
        util::sptr<stmt_base const> const consequence;
        util::sptr<stmt_base const> const alternative;
    };

    struct initialization
        : public stmt_base
    {
        initialization(int o, util::sptr<expr_base const> i)
            : offset(o)
            , init(std::move(i))
        {}

        void write() const;

        int const offset;
        util::sptr<expr_base const> const init;
    };

    struct func_ret
        : public stmt_base
    {
        explicit func_ret(util::sptr<expr_base const> r)
            : ret_val(std::move(r))
        {}

        void write() const;

        util::sptr<expr_base const> const ret_val;
    };

    struct func_ret_nothing
        : public stmt_base
    {
        void write() const;
    };

}

#endif /* __STEKIN_INSTANCE_STATEMENT_NODES_H__ */
