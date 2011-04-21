#ifndef __STEKIN_PROTO_STATEMENT_NODES_H__
#define __STEKIN_PROTO_STATEMENT_NODES_H__

#include "node-base.h"
#include "block.h"
#include "../instance/fwd-decl.h"

namespace proto {

    struct arithmetics
        : public Statement
    {
        arithmetics(misc::pos_type const& ps, util::sptr<Expression const> e)
            : Statement(ps)
            , expr(std::move(e))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;

        util::sptr<Expression const> const expr;
    };

    struct branch
        : public Statement
    {
        branch(misc::pos_type const& ps, util::sptr<Expression const> p, block c, block a)
            : Statement(ps)
            , _predicate(std::move(p))
            , _consequence(std::move(c))
            , _alternative(std::move(a))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;
    private:
        util::sptr<Expression const> const _predicate;
        block _consequence;
        block _alternative;
    };

    struct var_def
        : public Statement
    {
        var_def(misc::pos_type const& ps, std::string const& n, util::sptr<Expression const> i)
            : Statement(ps)
            , name(n)
            , init(std::move(i))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;

        std::string const name;
        util::sptr<Expression const> const init;
    };

    struct func_ret
        : public Statement
    {
        func_ret(misc::pos_type const& ps, util::sptr<Expression const> r)
            : Statement(ps)
            , ret_val(std::move(r))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;

        util::sptr<Expression const> const ret_val;
    };

    struct func_ret_nothing
        : public Statement
    {
        explicit func_ret_nothing(misc::pos_type const& ps)
            : Statement(ps)
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;
    };

}

#endif /* __STEKIN_PROTO_STATEMENT_NODES_H__ */
