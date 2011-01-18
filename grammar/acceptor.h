#ifndef __STACKENING_GRAMMAR_ACCEPTOR_H__
#define __STACKENING_GRAMMAR_ACCEPTOR_H__

#include "stmt-nodes.h"
#include "expr-nodes.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace grammar {

    struct acceptor {
        virtual void accept_stmt(util::sptr<stmt_base const> stmt) = 0;
        virtual void accept_func(util::sptr<func_def const> func) = 0;

        virtual void deliver_to(util::sref<acceptor> acc) = 0;

        virtual void accept_else(misc::pos_type const& else_pos);

        virtual ~acceptor() {}

        misc::pos_type const pos;
    protected:
        explicit acceptor(misc::pos_type const& ps)
            : pos(ps)
        {}

        acceptor(acceptor const&) = delete;
    };

    struct func_def_forbid_acceptor
        : public acceptor
    {
        void accept_func(util::sptr<func_def const> func);
    protected:
        explicit func_def_forbid_acceptor(misc::pos_type const& pos)
            : acceptor(pos)
        {}
    };

    struct if_acceptor
        : public func_def_forbid_acceptor
    {
        void accept_stmt(util::sptr<stmt_base const> stmt);

        void deliver_to(util::sref<acceptor> acc);

        void accept_else(misc::pos_type const& else_pos);

        if_acceptor(misc::pos_type const& pos, util::sptr<expr_base const> condition)
            : func_def_forbid_acceptor(pos)
            , _condition(std::move(condition))
            , _current_branch(&_valid)
        {}
    private:
        bool _else_matched() const;
    private:
        util::sptr<expr_base const> _condition;

        util::sptr<misc::pos_type> _last_else_pos;
        block* _current_branch;

        block _valid;
        block _invalid;
    };

    struct ifnot_acceptor
        : public func_def_forbid_acceptor
    {
        void accept_stmt(util::sptr<stmt_base const> stmt);

        void deliver_to(util::sref<acceptor> acc);

        ifnot_acceptor(misc::pos_type const& pos, util::sptr<expr_base const> condition)
            : func_def_forbid_acceptor(pos)
            , _condition(std::move(condition))
        {}
    private:
        util::sptr<expr_base const> _condition;

        block _invalid;
    };

    struct while_acceptor
        : public func_def_forbid_acceptor
    {
        void accept_stmt(util::sptr<stmt_base const> stmt);

        void deliver_to(util::sref<acceptor> acc);

        while_acceptor(misc::pos_type const& pos, util::sptr<expr_base const> condition)
            : func_def_forbid_acceptor(pos)
            , _condition(std::move(condition))
        {}
    private:
        util::sptr<expr_base const> _condition;

        block _body;
    };

    struct func_def_acceptor
        : public acceptor
    {
        void accept_stmt(util::sptr<stmt_base const> stmt);
        void accept_func(util::sptr<func_def const> func);

        void deliver_to(util::sref<acceptor> acc);

        func_def_acceptor(misc::pos_type const& pos
                        , std::string const& func_name
                        , std::vector<std::string> const& params)
            : acceptor(pos)
            , name(func_name)
            , param_names(params)
        {}

        std::string const name;
        std::vector<std::string> const param_names;
    private:
        block _body;
    };

}

#endif /* __STACKENING_GRAMMAR_ACCEPTOR_H__ */
