#ifndef __STEKIN_INSTANCE_STATEMENT_NODES_H__
#define __STEKIN_INSTANCE_STATEMENT_NODES_H__

#include "node-base.h"
#include "types.h"
#include "../util/pointer.h"

namespace inst {

    struct Arithmetics
        : public Statement
    {
        explicit Arithmetics(util::sptr<Expression const> e)
            : expr(std::move(e))
        {}

        void write() const;

        util::sptr<Expression const> const expr;
    };

    struct Branch
        : public Statement
    {
        Branch(util::sptr<Expression const> p
             , util::sptr<Statement const> c
             , util::sptr<Statement const> a)
                : predicate(std::move(p))
                , consequence(std::move(c))
                , alternative(std::move(a))
        {}

        void write() const;

        util::sptr<Expression const> const predicate;
        util::sptr<Statement const> const consequence;
        util::sptr<Statement const> const alternative;
    };

    struct Initialization
        : public Statement
    {
        Initialization(int o, util::sptr<Expression const> i, util::sptr<Type const> t)
            : offset(o)
            , init(std::move(i))
            , type(std::move(t))
        {}

        void write() const;

        int const offset;
        util::sptr<Expression const> const init;
        util::sptr<Type const> const type;
    };

    struct Return
        : public Statement
    {
        explicit Return(util::sptr<Expression const> r)
            : ret_val(std::move(r))
        {}

        void write() const;

        util::sptr<Expression const> const ret_val;
    };

    struct ReturnNothing
        : public Statement
    {
        void write() const;
    };

}

#endif /* __STEKIN_INSTANCE_STATEMENT_NODES_H__ */
