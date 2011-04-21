#ifndef __STAKCENING_PROTO_NODE_BASE_H__
#define __STAKCENING_PROTO_NODE_BASE_H__

#include "../instance/fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace proto {

    struct Expression {
        virtual ~Expression() {}

        virtual util::sptr<inst::Expression const> inst(util::sref<inst::scope> sc) const = 0;

        misc::pos_type const pos;
    protected:
        explicit Expression(misc::pos_type const ps)
            : pos(ps)
        {}
    };

    struct Statement {
        virtual ~Statement() {}

        virtual util::sptr<inst::mediate_base> inst(util::sref<inst::scope> sc) const = 0;

        misc::pos_type const pos;
    protected:
        explicit Statement(misc::pos_type const ps)
            : pos(ps)
        {}
    };

}

#endif /* __STAKCENING_PROTO_NODE_BASE_H__ */
