#ifndef __STEKIN_INSTANCE_STATEMENT_NODES_H__
#define __STEKIN_INSTANCE_STATEMENT_NODES_H__

#include "node-base.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

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
        Branch(misc::position const& pos
             , util::sptr<Expression const> p
             , util::sptr<Statement const> c
             , util::sptr<Statement const> a);

        void write() const;

        util::sptr<Expression const> const predicate;
        util::sptr<Statement const> const consequence;
        util::sptr<Statement const> const alternative;
    };

    struct initialization
        : public Statement
    {
        initialization(int o, util::sptr<Expression const> i)
            : offset(o)
            , init(std::move(i))
        {}

        void write() const;

        int const offset;
        util::sptr<Expression const> const init;
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
