#ifndef __STAKCENING_PROTO_NODE_BASE_H__
#define __STAKCENING_PROTO_NODE_BASE_H__

#include "../instance/node-base.h"
#include "../instance/scope.h"

namespace proto {

    struct expr_base {
        virtual ~expr_base() {}

        virtual util::sptr<inst::expr_base const> inst(util::sref<inst::scope const> sc) const = 0;
    protected:
        expr_base() {}
    };

    struct stmt_base {
        virtual ~stmt_base() {}

        virtual util::sptr<inst::stmt_base const> inst(util::sref<inst::scope const> sc) const = 0;
    protected:
        stmt_base() {}
    };

}

#endif /* __STAKCENING_PROTO_NODE_BASE_H__ */
