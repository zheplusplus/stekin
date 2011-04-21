#ifndef __STEKIN_GRAMMAR_ACCEPTOR_H__
#define __STEKIN_GRAMMAR_ACCEPTOR_H__

#include "node-base.h"
#include "function.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace grammar {

    struct acceptor {
        virtual void accept_func(util::sptr<Function const> func) = 0;
        virtual void accept_stmt(util::sptr<Statement const> stmt) = 0;
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

    struct if_acceptor
        : public acceptor
    {
        void accept_func(util::sptr<Function const> func);
        void accept_stmt(util::sptr<Statement const> stmt);
        void deliver_to(util::sref<acceptor> acc);
        void accept_else(misc::pos_type const& else_pos);

        if_acceptor(misc::pos_type const& pos, util::sptr<Expression const> predicate)
            : acceptor(pos)
            , _predicate(std::move(predicate))
            , _current_branch(&_consequence)
        {}
    private:
        bool _else_matched() const;
    private:
        util::sptr<Expression const> _predicate;

        util::sptr<misc::pos_type> _last_else_pos;
        Block* _current_branch;

        Block _consequence;
        Block _alternative;
    };

    struct ifnot_acceptor
        : public acceptor
    {
        void accept_func(util::sptr<Function const> func);
        void accept_stmt(util::sptr<Statement const> stmt);
        void deliver_to(util::sref<acceptor> acc);

        ifnot_acceptor(misc::pos_type const& pos, util::sptr<Expression const> predicate)
            : acceptor(pos)
            , _predicate(std::move(predicate))
        {}
    private:
        util::sptr<Expression const> _predicate;

        Block _alternative;
    };

    struct Function_acceptor
        : public acceptor
    {
        void accept_func(util::sptr<Function const> func);
        void accept_stmt(util::sptr<Statement const> stmt);
        void deliver_to(util::sref<acceptor> acc);

        Function_acceptor(misc::pos_type const& pos
                        , std::string const& func_name
                        , std::vector<std::string> const& params)
            : acceptor(pos)
            , name(func_name)
            , param_names(params)
        {}

        std::string const name;
        std::vector<std::string> const param_names;
    private:
        Block _body;
    };

}

#endif /* __STEKIN_GRAMMAR_ACCEPTOR_H__ */
