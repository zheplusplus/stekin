#ifndef __STEKIN_PROTO_BUILT_IN_H__
#define __STEKIN_PROTO_BUILT_IN_H__

#include "expr-nodes.h"

namespace proto {

    struct WriteExpr
        : public Expression
    {
        WriteExpr()
            : Expression(misc::position(0))
            , ref(misc::position(0), "value to write")
        {}

        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st);

        Reference ref;
    };

}

#endif /* __STEKIN_PROTO_BUILT_IN_H__ */
