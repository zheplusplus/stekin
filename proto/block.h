#ifndef __STEKIN_PROTO_BLOCK_H__
#define __STEKIN_PROTO_BLOCK_H__

#include <list>

#include "fwd-decl.h"
#include "node-base.h"
#include "../util/pointer.h"

namespace proto {

    struct Block
        : public Statement
    {
        Block(Block const&) = delete;

        Block() = default;

        void addStmt(util::sptr<Statement> stmt);
        util::sref<Function> declare(misc::position const& pos
                                   , std::string const& name
                                   , std::vector<std::string> const& param_names
                                   , bool contains_void_return);

        void addTo(util::sref<FuncInstDraft> func);
        util::sptr<inst::Statement const> inst(util::sref<FuncInstDraft> func
                                             , util::sref<SymbolTable> st);
        void mediateInst(util::sref<FuncInstDraft> func, util::sref<SymbolTable> st);

        std::vector<util::sptr<inst::Function const>> deliverFuncs();
    private:
        std::list<util::sptr<Statement>> _stmts;
        std::list<util::sptr<Function>> _funcs;
    };

}

#endif /* __STEKIN_PROTO_BLOCK_H__ */
