#ifndef __STEKIN_PROTO_BLOCK_H__
#define __STEKIN_PROTO_BLOCK_H__

#include <list>

#include "fwd-decl.h"
#include "../instance/fwd-decl.h"
#include "../util/pointer.h"

namespace proto {

    struct Block {
        Block(Block const&) = delete;
        Block() = default;

        Block(Block&& rhs)
            : _stmts(std::move(rhs._stmts))
        {}

        util::sptr<inst::MediateBase> inst(util::sref<inst::Scope> scope) const;

        void addStmt(util::sptr<Statement const> stmt);
        std::list<util::sptr<Statement const>> const& getStmts() const;
    private:
        std::list<util::sptr<Statement const>> _stmts;
    };

}

#endif /* __STEKIN_PROTO_BLOCK_H__ */
