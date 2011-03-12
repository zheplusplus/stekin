#ifndef __STACKENING_GRAMMAR_BLOCK_H__
#define __STACKENING_GRAMMAR_BLOCK_H__

#include <list>

#include "fwd-decl.h"
#include "../flowcheck/fwd-decl.h"
#include "../util/pointer.h"

namespace grammar {

    struct block {
        util::sptr<flchk::filter> compile(util::sptr<flchk::filter> filter) const;

        void add_stmt(util::sptr<stmt_base> stmt);
        void add_func(util::sptr<function const> func);

        block() = default;

        block(block const&) = delete;

        block(block&& rhs)
            : _stmts(std::move(rhs._stmts))
            , _funcs(std::move(rhs._funcs))
        {}
    private:
        std::list<util::sptr<stmt_base>> _stmts;
        std::list<util::sptr<function const>> _funcs;
    };

}

#endif /* __STACKENING_GRAMMAR_BLOCK_H__ */
