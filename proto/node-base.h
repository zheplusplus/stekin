#ifndef __STAKCENING_PROTO_NODE_BASE_H__
#define __STAKCENING_PROTO_NODE_BASE_H__

#include "../instance/fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace proto {

    struct expr_base {
        virtual ~expr_base() {}

        virtual util::sptr<inst::expr_base const> inst(util::sref<inst::scope> sc) const = 0;

        misc::pos_type const pos;
    protected:
        explicit expr_base(misc::pos_type const ps)
            : pos(ps)
        {}
    };

    struct stmt_base {
        virtual ~stmt_base() {}

        virtual util::sptr<inst::mediate_base> inst(util::sref<inst::scope> sc) const = 0;

        misc::pos_type const pos;
    protected:
        explicit stmt_base(misc::pos_type const ps)
            : pos(ps)
        {}
    };

}

#endif /* __STAKCENING_PROTO_NODE_BASE_H__ */
