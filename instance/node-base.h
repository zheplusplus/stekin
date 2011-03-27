#ifndef __STACKENING_INSTANCE_NODE_BASE_H__
#define __STACKENING_INSTANCE_NODE_BASE_H__

#include "fwd-decl.h"
#include "../util/pointer.h"

namespace inst {

    struct expr_base {
        virtual ~expr_base() {}

        virtual util::sref<type const> typeof() const = 0;
        virtual void write() const = 0;
    protected:
        expr_base() {}
    };

    struct stmt_base {
        virtual ~stmt_base() {}

        virtual void write() const = 0;
    protected:
        stmt_base() {}
    };

}

#endif /* __STACKENING_INSTANCE_NODE_BASE_H__ */
