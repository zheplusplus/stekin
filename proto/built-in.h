#ifndef __STEKIN_PROTO_BUILT_IN_H__
#define __STEKIN_PROTO_BUILT_IN_H__

#include "expr-nodes.h"

namespace proto {

    struct WriterExpr
        : public Expression
    {
        WriterExpr()
            : Expression(misc::position())
            , ref(misc::position(), "value to write")
        {}

        util::sref<Type const> type(util::sref<SymbolTable const>, misc::trace& trace) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st
                                              , misc::trace& trace) const;

        Reference ref;
    };

}

#endif /* __STEKIN_PROTO_BUILT_IN_H__ */
