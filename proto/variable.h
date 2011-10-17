#ifndef __STEKIN_PROTO_VARIABLE_H__
#define __STEKIN_PROTO_VARIABLE_H__

#include <vector>

#include <misc/pos-type.h>
#include <util/pointer.h>

#include "fwd-decl.h"

namespace proto {

    struct Variable {
        misc::position const def_pos;
        util::sref<Type const> const type;
        int const stack_offset;
        int const level;

        Variable(misc::position const& pos, util::sref<Type const> t, int offset, int lvl)
            : def_pos(pos)
            , type(t)
            , stack_offset(offset)
            , level(lvl)
        {}

        util::sref<FuncInstDraft> call(std::vector<util::sref<Type const>> const& arg_types
                                     , misc::trace& trace) const;

        Variable adjustLocation(int offset_diff, int lvl) const;

        bool operator<(Variable const& rhs) const;
        bool operator==(Variable const& rhs) const;
        bool operator!=(Variable const& rhs) const;
    };

}

#endif /* __STEKIN_PROTO_VARIABLE_H__ */
