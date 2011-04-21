#ifndef __STEKIN_GRAMMAR_ACCEPTOR_H__
#define __STEKIN_GRAMMAR_ACCEPTOR_H__

#include "node-base.h"
#include "function.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace grammar {

    struct Acceptor {
        virtual void accept_func(util::sptr<Function const> func) = 0;
        virtual void accept_stmt(util::sptr<Statement const> stmt) = 0;
        virtual void deliver_to(util::sref<Acceptor> acc) = 0;

        virtual void accept_else(misc::position const& else_pos);

        virtual ~Acceptor() {}

        misc::position const pos;
    protected:
        explicit Acceptor(misc::position const& ps)
            : pos(ps)
        {}

        Acceptor(Acceptor const&) = delete;
    };

    struct IfAcceptor
        : public Acceptor
    {
        void accept_func(util::sptr<Function const> func);
        void accept_stmt(util::sptr<Statement const> stmt);
        void deliver_to(util::sref<Acceptor> acc);
        void accept_else(misc::position const& else_pos);

        IfAcceptor(misc::position const& pos, util::sptr<Expression const> predicate)
            : Acceptor(pos)
            , _predicate(std::move(predicate))
            , _current_branch(&_consequence)
        {}
    private:
        bool _else_matched() const;
    private:
        util::sptr<Expression const> _predicate;

        util::sptr<misc::position> _last_else_pos;
        Block* _current_branch;

        Block _consequence;
        Block _alternative;
    };

    struct IfnotAcceptor
        : public Acceptor
    {
        void accept_func(util::sptr<Function const> func);
        void accept_stmt(util::sptr<Statement const> stmt);
        void deliver_to(util::sref<Acceptor> acc);

        IfnotAcceptor(misc::position const& pos, util::sptr<Expression const> predicate)
            : Acceptor(pos)
            , _predicate(std::move(predicate))
        {}
    private:
        util::sptr<Expression const> _predicate;

        Block _alternative;
    };

    struct FunctionAcceptor
        : public Acceptor
    {
        void accept_func(util::sptr<Function const> func);
        void accept_stmt(util::sptr<Statement const> stmt);
        void deliver_to(util::sref<Acceptor> acc);

        FunctionAcceptor(misc::position const& pos
                        , std::string const& func_name
                        , std::vector<std::string> const& params)
            : Acceptor(pos)
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
