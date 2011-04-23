#ifndef __STEKIN_INSTANCE_NODE_BASE_H__
#define __STEKIN_INSTANCE_NODE_BASE_H__

#include "fwd-decl.h"
#include "../util/pointer.h"

namespace inst {

    struct Expression {
        virtual ~Expression() {}

        virtual util::sref<Type const> typeof() const = 0;
        virtual void write() const = 0;
    protected:
        Expression() {}
    };

    struct Statement {
        virtual ~Statement() {}

        virtual void write() const = 0;
    protected:
        Statement() {}
    };

}

#endif /* __STEKIN_INSTANCE_NODE_BASE_H__ */
