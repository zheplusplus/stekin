#ifndef __STACKENING_PROTO_GLOBAL_SCOPE_H__
#define __STACKENING_PROTO_GLOBAL_SCOPE_H__

#include "general-scope.h"

namespace proto {

    struct global_scope
        : public general_scope
    {
        global_scope();
    };

}

#endif /* __STACKENING_PROTO_GLOBAL_SCOPE_H__ */
