#ifndef __STAKCENING_PROTO_NODE_BASE_H__
#define __STAKCENING_PROTO_NODE_BASE_H__

#include "../instance/node-base.h"
#include "../instance/scope.h"

namespace proto {

    struct expr_base {
        virtual ~expr_base() {}

        virtual inst::expr_base const* inst(inst::scope const* sc) const = 0;
    protected:
        expr_base() {}
    };

    enum termination_status {
        NO_EXPLICIT_TERMINATION,
        RETURN_VOID,
        RETURN_NO_VOID,
    };

    struct stmt_base {
        virtual ~stmt_base() {}

        virtual inst::stmt_base const* inst(inst::scope const* sc) const = 0;

        virtual termination_status termination() const
        {
            return NO_EXPLICIT_TERMINATION;
        }
    protected:
        stmt_base() {}
    };

}

#endif /* __STAKCENING_PROTO_NODE_BASE_H__ */
