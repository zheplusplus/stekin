#ifndef __STEKIN_INSTANCE_BLOCK_H__
#define __STEKIN_INSTANCE_BLOCK_H__

#include <list>

#include "fwd-decl.h"
#include "../util/pointer.h"

namespace inst {

    struct Block
        : public Statement
    {
        void addStmt(util::sptr<Statement const> stmt);

        Block() = default;

        Block(Block const&) = delete;

        Block(Block&& rhs)
            : _stmts(std::move(rhs._stmts))
        {}

        void write() const;
    private:
        std::list<util::sptr<Statement const>> _stmts;
    };

}

#endif /* __STEKIN_INSTANCE_BLOCK_H__ */
