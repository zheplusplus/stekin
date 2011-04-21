#ifndef __STEKIN_PROTO_BLOCK_H__
#define __STEKIN_PROTO_BLOCK_H__

#include <list>

#include "fwd-decl.h"
#include "../instance/fwd-decl.h"
#include "../util/pointer.h"

namespace proto {

    struct block {
        block(block const&) = delete;
        block() = default;

        block(block&& rhs)
            : _stmts(std::move(rhs._stmts))
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const;

        void add_stmt(util::sptr<Statement const> stmt);
        std::list<util::sptr<Statement const>> const& get_stmts() const;
    private:
        std::list<util::sptr<Statement const>> _stmts;
    };

}

#endif /* __STEKIN_PROTO_BLOCK_H__ */
