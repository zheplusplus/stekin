#ifndef __STEKIN_FLOWCHECK_BLOCK_H__
#define __STEKIN_FLOWCHECK_BLOCK_H__

#include <string>
#include <vector>

#include "fwd-decl.h"
#include "../proto/fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace flchk {

    struct Block {
        Block() = default;
        Block(Block const&) = delete;

        Block(Block&& rhs)
            : _stmts(std::move(rhs._stmts))
            , _funcs(std::move(rhs._funcs))
        {}

        void compile(util::sref<proto::Block> block, util::sref<SymbolTable> st) const;

        void addStmt(util::sptr<Statement const> stmt);
        util::sref<Function> defFunc(misc::position const& pos
                                   , std::string const& name
                                   , std::vector<std::string> const& param_names
                                   , util::sptr<Filter> body);
        void append(Block following);
    private:
        std::vector<util::sptr<Statement const>> _stmts;
        std::vector<util::sptr<Function>> _funcs;
    };

}

#endif /* __STEKIN_FLOWCHECK_BLOCK_H__ */
