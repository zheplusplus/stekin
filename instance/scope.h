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

        virtual void addPath(util::sref<MediateBase> path) = 0;
        virtual void instNextPath() = 0;
        virtual bool hasMorePath() const = 0;
    protected:
        Scope() {}
        void _addStmt(util::sptr<Statement const> stmt);

        Block _block;
    };

}

#endif /* __STEKIN_INSTNACE_SCOPE_H__ */
