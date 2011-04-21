#ifndef __STAKCENING_FLOW_CHECK_STATEMENT_NODES_H__
#define __STAKCENING_FLOW_CHECK_STATEMENT_NODES_H__

#include <string>
#include <list>

#include "node-base.h"
#include "block.h"
#include "../util/pointer.h"

namespace flchk {

    struct arithmetics
        : public Statement
    {
        arithmetics(misc::pos_type const& pos, util::sptr<Expression const> e)
            : Statement(pos)
            , expr(std::move(e))
        {}

        util::sptr<proto::Statement const> compile(util::sref<proto::scope> scope) const;

        util::sptr<Expression const> const expr;
    };

    struct branch
        : public Statement
    {
        branch(misc::pos_type const& pos, util::sptr<Expression const> p, Block c, Block a)
            : Statement(pos)
            , predicate(std::move(p))
            , consequence(std::move(c))
            , alternative(std::move(a))
        {}

        util::sptr<proto::Statement const> compile(util::sref<proto::scope> scope) const;

        util::sptr<Expression const> const predicate;
        Block const consequence;
        Block const alternative;
    };

    struct func_ret
        : public Statement
    {
        func_ret(misc::pos_type const& pos, util::sptr<Expression const> retval)
            : Statement(pos)
            , ret_val(std::move(retval))
        {}

        util::sptr<proto::Statement const> compile(util::sref<proto::scope> scope) const;

        util::sptr<Expression const> const ret_val;
    };

    struct func_ret_nothing
        : public Statement
    {
        explicit func_ret_nothing(misc::pos_type const& pos)
            : Statement(pos)
        {}

        util::sptr<proto::Statement const> compile(util::sref<proto::scope> scope) const;
    };

    struct var_def
        : public Statement
    {
        var_def(misc::pos_type const& pos, std::string const& var_name, util::sptr<Expression const> var_init)
            : Statement(pos)
            , name(var_name)
            , init(std::move(var_init))
        {}

        util::sptr<proto::Statement const> compile(util::sref<proto::scope> scope) const;

        std::string const name;
        util::sptr<Expression const> const init;
    };

}

#endif /* __STAKCENING_FLOW_CHECK_STATEMENT_NODES_H__ */
