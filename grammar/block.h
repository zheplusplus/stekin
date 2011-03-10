#ifndef __STACKENING_GRAMMAR_BLOCK_H__
#define __STACKENING_GRAMMAR_BLOCK_H__

#include <list>

#include "fwd-decl.h"
#include "../proto/fwd-decl.h"
#include "../util/pointer.h"

namespace grammar {

    struct block {
        void compile(util::sref<proto::scope> scope) const;

        void add_stmt(util::sptr<stmt_base const> stmt);
        void add_func(util::sptr<function const> function);

        block() = default;

        block(block const&) = delete;

        block(block&& rhs)
            : _flow(std::move(rhs._flow))
            , _functions(std::move(rhs._functions))
        {}
    private:
        std::list<util::sptr<stmt_base const>> _flow;
        std::list<util::sptr<function const>> _functions;
    };

}

#endif /* __STACKENING_GRAMMAR_BLOCK_H__ */
