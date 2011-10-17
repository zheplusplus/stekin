#ifndef __STEKIN_PROTO_LIST_CONTEXT_H__
#define __STEKIN_PROTO_LIST_CONTEXT_H__

#include "fwd-decl.h"

namespace proto {

    struct ListContext {
        explicit ListContext(util::sref<Type const> mt)
            : member_type(mt)
        {}

        util::sref<Type const> const member_type;
    };

}

#endif /* __STEKIN_PROTO_LIST_CONTEXT_H__ */
