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
             , util::sptr<expr_base const> p
             , util::sptr<block const> c
             , util::sptr<block const> a)
                : pos(ps)
                , predicate(std::move(p))
                , consequence(std::move(c))
                , alternative(std::move(a))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;

        misc::pos_type const pos;
        util::sptr<expr_base const> const predicate;
        util::sptr<block const> const consequence;
        util::sptr<block const> const alternative;
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
