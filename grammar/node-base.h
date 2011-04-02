#ifndef __STAKCENING_GRAMMAR_NODE_BASE_H__
#define __STAKCENING_GRAMMAR_NODE_BASE_H__

#include "../flowcheck/fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace grammar {

    struct stmt_base {
        misc::pos_type const pos;

        virtual void compile(util::sref<flchk::filter> scope) const = 0;

        virtual ~stmt_base() {}
    protected:
        explicit stmt_base(misc::pos_type const& ps)
            : pos(ps)
        {}

        stmt_base(stmt_base const&) = delete;
    };

    struct expr_base {
        misc::pos_type const pos;

        virtual util::sptr<flchk::expr_base const> compile() const = 0;

        virtual ~expr_base() {}
    protected:
        explicit expr_base(misc::pos_type const& ps)
            : pos(ps)
        {}

        expr_base(expr_base const&) = delete;
    };

}

#endif /* __STAKCENING_GRAMMAR_NODE_BASE_H__ */
