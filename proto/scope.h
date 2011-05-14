#ifndef __STEKIN_PROTO_SCOPE_H__
#define __STEKIN_PROTO_SCOPE_H__

#include <string>
#include <vector>
#include <list>
#include <gmpxx.h>

#include "fwd-decl.h"
#include "block.h"
#include "../misc/pos-type.h"

namespace proto {

    struct Scope {
        Scope(Scope const&) = delete;

        Scope()
            : _block(new Block)
        {}
        virtual ~Scope() {}
    public:
        util::sref<Function> declare(misc::position const& pos
                                   , std::string const& name
                                   , std::vector<std::string> const& param_names
                                   , bool contains_void_return);
    public:
        void addStmt(util::sptr<Statement> stmt);
    public:
        util::sptr<Statement> inst();
    protected:
        util::sptr<Block> _block;
    };

}

#endif /* __STEKIN_PROTO_SCOPE_H__ */
