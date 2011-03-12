#ifndef __STACKENING_FLOW_CHECK_NODE_BASE_H__
#define __STACKENING_FLOW_CHECK_NODE_BASE_H__

#include <string>
#include <vector>

#include "../proto/fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace flchk {

    struct stmt_base {
        misc::pos_type const pos;

        virtual util::sptr<proto::stmt_base const> compile(util::sref<proto::scope> scope) const = 0;

        virtual ~stmt_base() {}
    protected:
        explicit stmt_base(misc::pos_type const& ps)
            : pos(ps)
        {}

        stmt_base(stmt_base const&) = delete;
    };

    struct expr_base {
        misc::pos_type const pos;

        virtual util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const = 0;

        virtual ~expr_base() {}
    protected:
        expr_base(misc::pos_type const& ps)
            : pos(ps)
        {}

        expr_base(expr_base const&) = delete;
    };

}

#endif /* __STACKENING_FLOW_CHECK_NODE_BASE_H__ */