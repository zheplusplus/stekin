#ifndef __STACKENING_INSTANCE_BLOCK_H__
#define __STACKENING_INSTANCE_BLOCK_H__

#include <list>

#include "fwd-decl.h"
#include "../util/pointer.h"

namespace inst {

    struct block
        : public stmt_base
    {
        void add_stmt(util::sptr<stmt_base const> stmt);

        block() = default;

        block(block const&) = delete;

        block(block&& rhs)
            : _stmts(std::move(rhs._stmts))
        {}

        void write() const;
    private:
        std::list<util::sptr<stmt_base const>> _stmts;
    };

}

#endif /* __STACKENING_INSTANCE_BLOCK_H__ */
