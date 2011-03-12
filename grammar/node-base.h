#ifndef __STAKCENING_GRAMMAR_NODE_BASE_H__
#define __STAKCENING_GRAMMAR_NODE_BASE_H__

#include "../flowcheck/fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace grammar {

    struct stmt_base {
        misc::pos_type const pos;

        virtual void compile(util::sref<flchk::filter> scope) = 0;

        virtual ~stmt_base() {}
    protected:
        stmt_base(misc::pos_type const& ps)
            : pos(ps)
        {}

        stmt_base(stmt_base const&) = delete;
    };

    typedef flchk::expr_base expr_base;

}

#endif /* __STAKCENING_GRAMMAR_NODE_BASE_H__ */
