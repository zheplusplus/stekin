#ifndef __STACKENING_PROTO_BLOCK_H__
#define __STACKENING_PROTO_BLOCK_H__

#include <list>

#include "node-base.h"
#include "../instance/scope.h"

namespace proto {

    struct block
        : public stmt_base
    {
        block(block const&) = delete;
        block() = default;

        block(block&& rhs)
            : _stmts(std::move(rhs._stmts))
        {}

        void add_stmt(util::sptr<stmt_base const> stmt);

        util::sptr<inst::stmt_base const> inst(util::sref<inst::scope const> scope) const;
    private:
        std::list<util::sptr<stmt_base const>> _stmts;
    };

}

#endif /* __STACKENING_PROTO_BLOCK_H__ */
