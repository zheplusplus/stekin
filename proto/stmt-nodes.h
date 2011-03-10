#ifndef __STACKENING_PROTO_STATEMENT_NODES_H__
#define __STACKENING_PROTO_STATEMENT_NODES_H__

#include "node-base.h"
#include "block.h"
#include "../instance/fwd-decl.h"

namespace proto {

    struct arithmetics
        : public stmt_base
    {
        arithmetics(misc::pos_type const& ps, util::sptr<expr_base const> e)
            : stmt_base(ps)
            , expr(std::move(e))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;

        util::sptr<expr_base const> const expr;
    };

    struct branch
        : public stmt_base
    {
        branch(misc::pos_type const& ps, util::sptr<expr_base const> p, block c, block a)
            : stmt_base(ps)
            , _predicate(std::move(p))
            , _consequence(std::move(c))
            , _alternative(std::move(a))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;
    private:
        util::sptr<expr_base const> const _predicate;
        block _consequence;
        block _alternative;
    };

    struct var_def
        : public stmt_base
    {
        var_def(misc::pos_type const& ps, std::string const& n, util::sptr<expr_base const> i)
            : stmt_base(ps)
            , name(n)
            , init(std::move(i))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;

        std::string const name;
        util::sptr<expr_base const> const init;
    };

    struct func_ret
        : public stmt_base
    {
        func_ret(misc::pos_type const& ps, util::sptr<expr_base const> r)
            : stmt_base(ps)
            , ret_val(std::move(r))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;

        util::sptr<expr_base const> const ret_val;
    };

    struct func_ret_nothing
        : public stmt_base
    {
        explicit func_ret_nothing(misc::pos_type const& ps)
            : stmt_base(ps)
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;
    };

}

#endif /* __STACKENING_PROTO_STATEMENT_NODES_H__ */
