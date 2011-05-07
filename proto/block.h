#ifndef __STEKIN_PROTO_BLOCK_H__
#define __STEKIN_PROTO_BLOCK_H__

#include <list>

#include "fwd-decl.h"
#include "../instance/inst-mediate.h"
#include "../util/pointer.h"

namespace proto {

    struct Block
        : public inst::MediateBase
    {
        Block(Block const&) = delete;

        Block() = default;

        void addMediate(util::sptr<inst::MediateBase> mediate);
        void addFunc(util::sptr<Function> func);

        void addTo(util::sref<inst::Scope> scope);
        util::sptr<inst::Statement const> inst(util::sref<inst::Scope> scope
                                             , util::sref<inst::SymbolTable> st);
        void mediateInst(util::sref<inst::Scope> scope, util::sref<inst::SymbolTable> st);
    private:
        std::list<util::sptr<inst::MediateBase>> _mediates;
        std::list<util::sptr<Function>> _funcs;
    };

}

#endif /* __STEKIN_PROTO_BLOCK_H__ */
