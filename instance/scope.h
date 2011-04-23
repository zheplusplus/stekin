#ifndef __STEKIN_INSTNACE_SCOPE_H__
#define __STEKIN_INSTNACE_SCOPE_H__

#include <string>

#include "fwd-decl.h"
#include "block.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"
#include "../proto/fwd-decl.h"

namespace inst {

    struct Scope {
        virtual ~Scope() {}
    public:
        virtual void setReturnType(misc::position const& pos, util::sref<Type const> type) = 0;

        virtual Variable defVar(misc::position const& pos
                               , util::sref<Type const> type
                               , std::string const& name) = 0;
        virtual Variable queryVar(misc::position const& pos, std::string const& name) const = 0;

        virtual void addPath(util::sref<MediateBase> path) = 0;
        virtual void instNextPath() = 0;
        virtual bool hasMorePath() const = 0;

        virtual int level() const = 0;

        operation const* queryBinary(misc::position const& pos
                                    , std::string const& op
                                    , util::sref<Type const> lhs
                                    , util::sref<Type const> rhs) const;
        operation const* queryPreUnary(misc::position const& pos
                                       , std::string const& op
                                       , util::sref<Type const> rhs) const;

        void addStmt(util::sptr<Statement const> stmt);
    protected:
        Scope() {}

        Block _block;
    };

}

#endif /* __STEKIN_INSTNACE_SCOPE_H__ */
