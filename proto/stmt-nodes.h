#ifndef __STACKENING_PROTO_STATEMENT_NODES_H__
#define __STACKENING_PROTO_STATEMENT_NODES_H__

#include <list>

#include "node-base.h"
#include "../instance/scope.h"
#include "../instance/stmt-nodes.h"
#include "../instance/block.h"

namespace proto {

    struct block
        : public stmt_base
    {
        explicit block(std::list<util::sptr<stmt_base const>> stmts)
            : _stmts(std::move(stmts))
            , _tstatus(NO_EXPLICIT_TERMINATION)
        {}

        util::sptr<inst::stmt_base const> inst(util::sref<inst::scope const> scope) const;

        termination_status termination() const
        {
            return _tstatus;
        }
    private:
        void _set_term_status(termination_status status);

        std::list<util::sptr<stmt_base const>> _stmts;
        termination_status _tstatus;
    };

    struct arithmetics
        : public stmt_base
    {
        arithmetics(misc::pos_type const& ps, util::sptr<expr_base const> e)
            : pos(ps)
            , expr(std::move(e))
        {}

        util::sptr<inst::stmt_base const> inst(util::sref<inst::scope const> scope) const;

        misc::pos_type const pos;
        util::sptr<expr_base const> const expr;
    };

    struct branch
        : public stmt_base
    {
        misc::pos_type const pos;
        util::sptr<expr_base const> const condition;
        util::sptr<stmt_base const> const valid;
        util::sptr<stmt_base const> const invalid;

        util::sptr<inst::stmt_base const> inst(util::sref<inst::scope const> scope) const;
        termination_status termination() const;

        branch(misc::pos_type const& ps
             , util::sptr<expr_base const> c
             , util::sptr<stmt_base const> v
             , util::sptr<stmt_base const> i)
                : pos(ps)
                , condition(std::move(c))
                , valid(std::move(v))
                , invalid(std::move(i))
        {}
    };

    struct loop
        : public stmt_base
    {
        misc::pos_type const pos;
        util::sptr<expr_base const> const condition;
        util::sptr<stmt_base const> const body;

        util::sptr<inst::stmt_base const> inst(util::sref<inst::scope const> scope) const;

        termination_status termination() const
        {
            return body->termination();
        }

        loop(misc::pos_type const& ps, util::sptr<expr_base const> c, util::sptr<stmt_base const> b)
            : pos(ps)
            , condition(std::move(c))
            , body(std::move(b))
        {}
    };

    struct var_def
        : public stmt_base
    {
        var_def(misc::pos_type const& ps, std::string const& n, util::sptr<expr_base const> i)
            : pos(ps)
            , name(n)
            , init(std::move(i))
        {}

        util::sptr<inst::stmt_base const> inst(util::sref<inst::scope const> scope) const;

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

        util::sptr<inst::stmt_base const> inst(util::sref<inst::scope const> scope) const;

        misc::pos_type const pos;

        termination_status termination() const
        {
            return RETURN_NO_VOID;
        }

        util::sptr<expr_base const> const ret_val;
    };

    struct func_ret_nothing
        : public stmt_base
    {
        explicit func_ret_nothing(misc::pos_type const& ps)
            : pos(ps)
        {}

        util::sptr<inst::stmt_base const> inst(util::sref<inst::scope const> scope) const;

        misc::pos_type const pos;

        termination_status termination() const
        {
            return RETURN_VOID;
        }
    };

}

#endif /* __STACKENING_PROTO_STATEMENT_NODES_H__ */
