#ifndef __STAKCENING_GRAMMAR_STATEMENT_NODES_H__
#define __STAKCENING_GRAMMAR_STATEMENT_NODES_H__

#include <string>
#include <list>

#include "node-base.h"
#include "../proto/func-templ.h"
#include "../util/pointer.h"

namespace grammar {

    struct func_def;

    struct block {
        void compile(util::sref<proto::scope> scope) const;

        void add_stmt(util::sptr<stmt_base const> stmt);
        void add_func(util::sptr<func_def const> func_def);

        block() = default;

        block(block const&) = delete;

        block(block&& rhs)
            : _flow(std::move(rhs._flow))
            , _func_defs(std::move(rhs._func_defs))
        {}
    private:
        std::list<util::sptr<stmt_base const>> _flow;
        std::list<util::sptr<func_def const>> _func_defs;
    };

    struct arithmetics
        : public stmt_base
    {
        arithmetics(misc::pos_type const& pos, util::sptr<expr_base const> e)
            : stmt_base(pos)
            , expr(std::move(e))
        {}

        void compile(util::sref<proto::scope> scope) const;

        util::sptr<expr_base const> const expr;
    };

    struct branch
        : public stmt_base
    {
        branch(misc::pos_type const& pos, util::sptr<expr_base const> cond, block v, block i)
            : stmt_base(pos)
            , condition(std::move(cond))
            , valid(std::move(v))
            , invalid(std::move(i))
        {}

        void compile(util::sref<proto::scope> scope) const;

        util::sptr<expr_base const> const condition;
        block const valid;
        block const invalid;
    };

    struct func_ret
        : public stmt_base
    {
        func_ret(misc::pos_type const& pos, util::sptr<expr_base const> retval)
            : stmt_base(pos)
            , ret_val(std::move(retval))
        {}

        void compile(util::sref<proto::scope> scope) const;

        util::sptr<expr_base const> const ret_val;
    };

    struct func_ret_nothing
        : public stmt_base
    {
        explicit func_ret_nothing(misc::pos_type const& pos)
            : stmt_base(pos)
        {}

        void compile(util::sref<proto::scope> scope) const;
    };

    struct var_def
        : public stmt_base
    {
        var_def(misc::pos_type const& pos, std::string const& var_name, util::sptr<expr_base const> var_init)
            : stmt_base(pos)
            , name(var_name)
            , init(std::move(var_init))
        {}

        void compile(util::sref<proto::scope> scope) const;

        std::string const name;
        util::sptr<expr_base const> const init;
    };

    struct func_def {
        func_def(misc::pos_type const& ps
               , std::string const& func_name
               , std::vector<std::string> const& params
               , block func_body)
            : pos(ps)
            , name(func_name)
            , param_names(params)
            , body(std::move(func_body))
        {}

        util::sref<proto::func_templ> declare(util::sref<proto::scope> scope) const;
        void compile(util::sref<proto::func_templ> templ) const;

        misc::pos_type const pos;

        std::string const name;
        std::vector<std::string> const param_names;
        block const body;
    };

}

#endif /* __STAKCENING_GRAMMAR_STATEMENT_NODES_H__ */
