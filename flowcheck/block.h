#ifndef __STACKENING_FLOW_CHECK_BLOCK_H__
#define __STACKENING_FLOW_CHECK_BLOCK_H__

#include <string>
#include <vector>

#include "fwd-decl.h"
#include "../proto/block.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace flchk {

    struct block {
        block() = default;
        block(block const&) = delete;

        block(block&& rhs)
            : _stmts(std::move(rhs._stmts))
            , _funcs(std::move(rhs._funcs))
        {}

        void compile(util::sref<proto::scope> scope) const;

        void add_stmt(util::sptr<stmt_base const> stmt);
        void def_func(misc::pos_type const& pos
                    , std::string const& name
                    , std::vector<std::string> const& param_names
                    , block body
                    , bool contains_void_return);
    private:
        std::list<util::sptr<stmt_base const>> _stmts;
        std::list<util::sptr<function const>> _funcs;
    };

}

#endif /* __STACKENING_FLOW_CHECK_BLOCK_H__ */
