#ifndef __STACKENING_INSTNACE_SCOPE_H__
#define __STACKENING_INSTNACE_SCOPE_H__

#include <string>

#include "type.h"
#include "node-base.h"
#include "operation.h"
#include "symbol-table.h"
#include "function.h"
#include "variable.h"
#include "block.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace inst {

    struct scope {
        scope(util::sref<function> func, util::sref<symbol_table> symbols)
            : _func(func)
            , _symbols(symbols)
        {}
    public:
        void set_return_type(misc::pos_type const& pos, type const* type) const;

        variable def_var(misc::pos_type const& pos, type const* vtype, std::string const& name) const;
        variable query_var(misc::pos_type const& pos, std::string const& name) const;

        operation const* query_binary(misc::pos_type const& pos
                                    , std::string const& op
                                    , type const* lhs
                                    , type const* rhs) const;
        operation const* query_pre_unary(misc::pos_type const& pos
                                       , std::string const& op
                                       , type const* rhs) const;

        void add_stmt(util::sptr<stmt_base const> stmt);

        int level() const;
    private:
        util::sref<function> _func;
        util::sref<symbol_table> _symbols;
        block _block;
    };

}

#endif /* __STACKENING_INSTNACE_SCOPE_H__ */
