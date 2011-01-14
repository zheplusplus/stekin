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
        block()
            : _tstatus(NO_EXPLICIT_TERMINATION)
        {}

        inst::block const* inst(inst::scope const* scope) const;

        void add_stmt(stmt_base const* stmt);

        termination_status termination() const
        {
            return _tstatus;
        }
    private:
        void _set_term_status(termination_status status);

        std::list<stmt_base const*> _stmts;
        termination_status _tstatus;
    };

    struct arithmetics
        : public stmt_base
    {
        arithmetics(misc::pos_type const& ps, expr_base const* e)
            : pos(ps)
            , expr(e)
        {}

        inst::arithmetics const* inst(inst::scope const* scope) const;

        misc::pos_type const pos;
        expr_base const* const expr;
    };

    struct branch
        : public stmt_base
    {
        misc::pos_type const pos;
        expr_base const* const condition;
        stmt_base const* const valid;
        stmt_base const* const invalid;

        inst::branch const* inst(inst::scope const* scope) const;
        termination_status termination() const;

        branch(misc::pos_type const& ps, expr_base const* c, stmt_base const* v, stmt_base const* i)
            : pos(ps)
            , condition(c)
            , valid(v)
            , invalid(i)
        {}
    };

    struct loop
        : public stmt_base
    {
        misc::pos_type const pos;
        expr_base const* const condition;
        stmt_base const* const body;

        inst::loop const* inst(inst::scope const* scope) const;

        termination_status termination() const
        {
            return body->termination();
        }

        loop(misc::pos_type const& ps, expr_base const* c, stmt_base const* b)
            : pos(ps)
            , condition(c)
            , body(b)
        {}
    };

    struct var_def
        : public stmt_base
    {
        var_def(misc::pos_type const& ps, std::string const& n, expr_base const* i)
            : pos(ps)
            , name(n)
            , init(i)
        {}

        inst::initialization const* inst(inst::scope const* scope) const;

        misc::pos_type const pos;
        std::string const name;
        expr_base const* const init;
    };

    struct func_ret
        : public stmt_base
    {
        func_ret(misc::pos_type const& ps, expr_base const* r)
            : pos(ps)
            , ret_val(r)
        {}

        inst::func_ret const* inst(inst::scope const* scope) const;

        misc::pos_type const pos;

        termination_status termination() const
        {
            return RETURN_NO_VOID;
        }

        expr_base const* const ret_val;
    };

    struct func_ret_nothing
        : public stmt_base
    {
        explicit func_ret_nothing(misc::pos_type const& ps)
            : pos(ps)
        {}

        inst::func_ret_nothing const* inst(inst::scope const* scope) const;

        misc::pos_type const pos;

        termination_status termination() const
        {
            return RETURN_VOID;
        }
    };

}

#endif /* __STACKENING_PROTO_STATEMENT_NODES_H__ */
