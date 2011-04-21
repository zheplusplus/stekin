#ifndef __STEKIN_INSTANCE_STATEMENT_NODES_H__
#define __STEKIN_INSTANCE_STATEMENT_NODES_H__

#include "node-base.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace inst {

    struct arithmetics
        : public Statement
    {
        explicit arithmetics(util::sptr<Expression const> e)
            : expr(std::move(e))
        {}

        void write() const;

        util::sptr<Expression const> const expr;
    };

    struct branch
        : public Statement
    {
        branch(misc::pos_type const& pos
             , util::sptr<Expression const> p
             , util::sptr<Statement const> c
             , util::sptr<Statement const> a);

        void write() const;

        util::sptr<Expression const> const predicate;
        util::sptr<Statement const> const consequence;
        util::sptr<Statement const> const alternative;
    };

    struct initialization
        : public Statement
    {
        initialization(int o, util::sptr<Expression const> i)
            : offset(o)
            , init(std::move(i))
        {}

        void write() const;

        int const offset;
        util::sptr<Expression const> const init;
    };

    struct func_ret
        : public Statement
    {
        explicit func_ret(util::sptr<Expression const> r)
            : ret_val(std::move(r))
        {}

        void write() const;

        util::sptr<Expression const> const ret_val;
    };

    struct func_ret_nothing
        : public Statement
    {
        void write() const;
    };

}

#endif /* __STEKIN_INSTANCE_STATEMENT_NODES_H__ */
