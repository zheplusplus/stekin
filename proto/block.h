#ifndef __STACKENING_PROTO_BLOCK_H__
#define __STACKENING_PROTO_BLOCK_H__

#include <list>

#include "node-base.h"
#include "../instance/scope.h"

namespace proto {

    struct block {
        block(block const&) = delete;
        block() = default;

        block(block&& rhs)
            : _stmts(std::move(rhs._stmts))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;

        void add_stmt(util::sptr<stmt_base const> stmt);
        std::list<util::sptr<stmt_base const>> const& get_stmts() const;
    private:
        std::list<util::sptr<stmt_base const>> _stmts;
    };

}

#endif /* __STACKENING_PROTO_BLOCK_H__ */
