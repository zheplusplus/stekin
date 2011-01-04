#ifndef __STAKCENING_GRAMMAR_NODE_BASE_H__
#define __STAKCENING_GRAMMAR_NODE_BASE_H__

#include "../proto/scope.h"
#include "../misc/pos-type.h"

namespace grammar {

    struct stmt_base {
        misc::pos_type const pos;

        virtual void compile(proto::scope const* scope) const = 0;

        virtual ~stmt_base() {}
    protected:
        stmt_base(misc::pos_type const& ps)
            : pos(ps)
        {}

        stmt_base(stmt_base const&) = delete;
    };

    struct expr_base {
        misc::pos_type const pos;

        virtual proto::expr_prototype const* compile(proto::scope const* scope) const = 0;

        virtual ~expr_base() {}
    protected:
        expr_base(misc::pos_type const& ps)
            : pos(ps)
        {}

        expr_base(expr_base const&) = delete;
    };

}

#endif /* __STAKCENING_GRAMMAR_NODE_BASE_H__ */
