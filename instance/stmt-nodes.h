#ifndef __STEKIN_INSTANCE_STATEMENT_NODES_H__
#define __STEKIN_INSTANCE_STATEMENT_NODES_H__

#include <util/pointer.h>

#include "node-base.h"
#include "types.h"

namespace inst {

    struct Arithmetics
        : public Statement
    {
        Arithmetics(int l, util::sptr<Expression const> e)
            : level(l)
            , expr(std::move(e))
        {}

        void write() const;

        int const level;
        util::sptr<Expression const> const expr;
    };

    struct Branch
        : public Statement
    {
        Branch(int l
             , util::sptr<Expression const> p
             , util::sptr<Statement const> c
             , util::sptr<Statement const> a)
                : level(l)
                , predicate(std::move(p))
                , consequence(std::move(c))
                , alternative(std::move(a))
        {}

        void write() const;

        int const level;
        util::sptr<Expression const> const predicate;
        util::sptr<Statement const> const consequence;
        util::sptr<Statement const> const alternative;
    };

    struct Initialization
        : public Statement
    {
        Initialization(int l, int o, util::sptr<Expression const> i, util::sptr<Type const> t)
            : level(l)
            , offset(o)
            , init(std::move(i))
            , type(std::move(t))
        {}

        void write() const;

        int const level;
        int const offset;
        util::sptr<Expression const> const init;
        util::sptr<Type const> const type;
    };

    struct Return
        : public Statement
    {
        explicit Return(int l, util::sptr<Expression const> r)
            : level(l)
            , ret_val(std::move(r))
        {}

        void write() const;

        int const level;
        util::sptr<Expression const> const ret_val;
    };

    struct ReturnNothing
        : public Statement
    {
        void write() const;
    };

}

#endif /* __STEKIN_INSTANCE_STATEMENT_NODES_H__ */
