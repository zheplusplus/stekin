#ifndef __STEKIN_PROTO_STATEMENT_NODES_H__
#define __STEKIN_PROTO_STATEMENT_NODES_H__

#include "node-base.h"
#include "block.h"
#include "../instance/fwd-decl.h"

namespace proto {

    struct Arithmetics
        : public Statement
    {
        Arithmetics(misc::position const& ps, util::sptr<Expression const> e)
            : Statement(ps)
            , expr(std::move(e))
        {}

        util::sptr<inst::MediateBase> inst(util::sref<inst::Scope> scope) const;

        util::sptr<Expression const> const expr;
    };

    struct Branch
        : public Statement
    {
        Branch(misc::position const& ps, util::sptr<Expression const> p, Block c, Block a)
            : Statement(ps)
            , _predicate(std::move(p))
            , _consequence(std::move(c))
            , _alternative(std::move(a))
        {}

        util::sptr<inst::MediateBase> inst(util::sref<inst::Scope> scope) const;
    private:
        util::sptr<Expression const> const _predicate;
        Block _consequence;
        Block _alternative;
    };

    struct VarDef
        : public Statement
    {
        VarDef(misc::position const& ps, std::string const& n, util::sptr<Expression const> i)
            : Statement(ps)
            , name(n)
            , init(std::move(i))
        {}

        util::sptr<inst::MediateBase> inst(util::sref<inst::Scope> scope) const;

        std::string const name;
        util::sptr<Expression const> const init;
    };

    struct Return
        : public Statement
    {
        Return(misc::position const& ps, util::sptr<Expression const> r)
            : Statement(ps)
            , ret_val(std::move(r))
        {}

        util::sptr<inst::MediateBase> inst(util::sref<inst::Scope> scope) const;

        util::sptr<Expression const> const ret_val;
    };

    struct ReturnNothing
        : public Statement
    {
        explicit ReturnNothing(misc::position const& ps)
            : Statement(ps)
        {}

        util::sptr<inst::MediateBase> inst(util::sref<inst::Scope> scope) const;
    };

}

#endif /* __STEKIN_PROTO_STATEMENT_NODES_H__ */
