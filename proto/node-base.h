#ifndef __STEKIN_PROTO_NODE_BASE_H__
#define __STEKIN_PROTO_NODE_BASE_H__

#include <vector>

#include <instance/fwd-decl.h>
#include <util/pointer.h>
#include <misc/pos-type.h>

#include "fwd-decl.h"

namespace proto {

    struct Expression {
        virtual ~Expression() {}

        virtual util::sref<Type const> type(util::sref<SymbolTable const> st
                                          , misc::trace& trace) const = 0;
        virtual util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st
                                                      , misc::trace& trace) const = 0;
        virtual util::sref<Type const> typeAsPipe(util::sref<SymbolTable const> st
                                                , util::sref<ListContext const> lc
                                                , misc::trace& trace) const;
        virtual util::sptr<inst::Expression const> instAsPipe(util::sref<SymbolTable const> st
                                                            , util::sref<ListContext const> lc
                                                            , misc::trace& trace) const;

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
                                                     , misc::trace& trace) = 0;
        virtual void mediateInst(util::sref<FuncInstDraft> func, misc::trace& trace) = 0;
        virtual std::vector<util::sptr<inst::Function const>> deliverFuncs() = 0;
    protected:
        Statement() {}
    };

}

#endif /* __STEKIN_PROTO_NODE_BASE_H__ */
