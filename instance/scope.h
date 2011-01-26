#ifndef __STACKENING_INSTNACE_SCOPE_H__
#define __STACKENING_INSTNACE_SCOPE_H__

#include <string>

#include "type.h"
#include "node-base.h"
#include "operation.h"
#include "variable.h"
#include "block.h"
#include "inst-mediate.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace inst {

    struct scope {
        virtual ~scope() {}
    public:
        virtual void set_return_type(misc::pos_type const& pos, type const* type) = 0;

        virtual variable def_var(misc::pos_type const& pos, type const* vtype, std::string const& name) = 0;
        virtual variable query_var(misc::pos_type const& pos, std::string const& name) const = 0;

        virtual void add_path(util::sref<mediate_base> path) = 0;
        virtual void inst_next_path() = 0;

        virtual int level() const = 0;

        operation const* query_binary(misc::pos_type const& pos
                                    , std::string const& op
                                    , type const* lhs
                                    , type const* rhs) const;
        operation const* query_pre_unary(misc::pos_type const& pos
                                       , std::string const& op
                                       , type const* rhs) const;

        void add_stmt(util::sptr<stmt_base const> stmt);
    protected:
        scope() {}

        block _block;
    };

}

#endif /* __STACKENING_INSTNACE_SCOPE_H__ */
