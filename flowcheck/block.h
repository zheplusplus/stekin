#ifndef __STEKIN_FLOW_CHECK_BLOCK_H__
#define __STEKIN_FLOW_CHECK_BLOCK_H__

#include <string>
#include <vector>

#include "fwd-decl.h"
#include "../proto/block.h"
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

        void compile(util::sref<proto::Scope> scope) const;

        void addStmt(util::sptr<Statement const> stmt);
        void defFunc(misc::position const& pos
                   , std::string const& name
                   , std::vector<std::string> const& param_names
                   , Block body
                   , bool contains_void_return);
        void append(Block following);
    private:
        std::list<util::sptr<Statement const>> _stmts;
        std::list<util::sptr<Function const>> _funcs;
    };

}

#endif /* __STEKIN_FLOW_CHECK_BLOCK_H__ */
