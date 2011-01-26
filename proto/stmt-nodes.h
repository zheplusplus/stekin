#ifndef __STACKENING_PROTO_STATEMENT_NODES_H__
#define __STACKENING_PROTO_STATEMENT_NODES_H__

#include "node-base.h"
#include "block.h"
#include "../instance/scope.h"
#include "../instance/stmt-nodes.h"
#include "../instance/block.h"

namespace proto {

    struct arithmetics
        : public stmt_base
    {
        arithmetics(misc::pos_type const& ps, util::sptr<expr_base const> e)
            : pos(ps)
            , expr(std::move(e))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;

        misc::pos_type const pos;
        util::sptr<expr_base const> const expr;
    };

    struct branch
        : public stmt_base
    {
        branch(misc::pos_type const& ps
             , util::sptr<expr_base const> c
             , util::sptr<block const> v
             , util::sptr<block const> i)
                : pos(ps)
                , condition(std::move(c))
                , valid(std::move(v))
                , invalid(std::move(i))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;

        misc::pos_type const pos;
        util::sptr<expr_base const> const condition;
        util::sptr<block const> const valid;
        util::sptr<block const> const invalid;
    };

    struct loop
        : public stmt_base
    {
        loop(misc::pos_type const& ps, util::sptr<expr_base const> c, util::sptr<block const> b)
            : pos(ps)
            , condition(std::move(c))
            , body(std::move(b))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;

        misc::pos_type const pos;
        util::sptr<expr_base const> const condition;
        util::sptr<block const> const body;
    };

    struct var_def
        : public stmt_base
    {
        var_def(misc::pos_type const& ps, std::string const& n, util::sptr<expr_base const> i)
            : pos(ps)
            , name(n)
            , init(std::move(i))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;

        misc::pos_type const pos;
        std::string const name;
        util::sptr<expr_base const> const init;
    };

    struct func_ret
        : public stmt_base
    {
        func_ret(misc::pos_type const& ps, util::sptr<expr_base const> r)
            : pos(ps)
            , ret_val(std::move(r))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;

        misc::pos_type const pos;
        util::sptr<expr_base const> const ret_val;
    };

    struct func_ret_nothing
        : public stmt_base
    {
        explicit func_ret_nothing(misc::pos_type const& ps)
            : pos(ps)
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;

        misc::pos_type const pos;
    };

}

#endif /* __STACKENING_PROTO_STATEMENT_NODES_H__ */
