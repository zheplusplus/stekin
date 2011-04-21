#ifndef __STEKIN_INSTNACE_SCOPE_H__
#define __STEKIN_INSTNACE_SCOPE_H__

#include <string>

#include "fwd-decl.h"
#include "variable.h"
#include "block.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"
#include "../proto/fwd-decl.h"

namespace inst {

    struct scope {
        virtual ~scope() {}
    public:
        virtual void set_return_type(misc::position const& pos, util::sref<type const> type) = 0;

        virtual variable defVar(misc::position const& pos
                               , util::sref<type const> vtype
                               , std::string const& name) = 0;
        virtual variable query_var(misc::position const& pos, std::string const& name) const = 0;

        virtual void add_path(util::sref<mediate_base> path) = 0;
        virtual void inst_next_path() = 0;
        virtual bool has_more_path() const = 0;

        virtual int level() const = 0;

        operation const* query_binary(misc::position const& pos
                                    , std::string const& op
                                    , util::sref<type const> lhs
                                    , util::sref<type const> rhs) const;
        operation const* query_pre_unary(misc::position const& pos
                                       , std::string const& op
                                       , util::sref<type const> rhs) const;

        void addStmt(util::sptr<Statement const> stmt);
    protected:
        scope() {}

        Block _block;
    };

}

#endif /* __STEKIN_INSTNACE_SCOPE_H__ */
