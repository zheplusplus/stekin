#ifndef __STEKIN_PROTO_NODE_BASE_H__
#define __STEKIN_PROTO_NODE_BASE_H__

#include "../instance/fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace proto {

    struct Expression {
        virtual ~Expression() {}

        virtual util::sptr<inst::Expression const> inst(util::sref<inst::Scope> sc) const = 0;

        misc::position const pos;
    protected:
        explicit Expression(misc::position const ps)
            : pos(ps)
        {}
    };

    struct Statement {
        virtual ~Statement() {}

        virtual util::sptr<inst::MediateBase> inst(util::sref<inst::Scope> sc) const = 0;

        misc::position const pos;
    protected:
        explicit Statement(misc::position const ps)
            : pos(ps)
        {}
    };

}

#endif /* __STEKIN_PROTO_NODE_BASE_H__ */
