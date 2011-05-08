#ifndef __STEKIN_PROTO_NODE_BASE_H__
#define __STEKIN_PROTO_NODE_BASE_H__

#include "fwd-decl.h"
#include "../instance/fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace proto {

    struct Expression {
        virtual ~Expression() {}

        virtual util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st) = 0;

        misc::position const pos;
    protected:
        explicit Expression(misc::position const ps)
            : pos(ps)
        {}
    };

    struct Statement {
        virtual ~Statement() {}

        virtual void addTo(util::sref<FuncInstDraft> func) = 0;
        virtual util::sptr<inst::Statement const> inst(util::sref<FuncInstDraft> func
                                                     , util::sref<SymbolTable> st) = 0;
        virtual void mediateInst(util::sref<FuncInstDraft> func
                               , util::sref<SymbolTable> st) = 0;
    protected:
        Statement() {}
    };

}

#endif /* __STEKIN_PROTO_NODE_BASE_H__ */
