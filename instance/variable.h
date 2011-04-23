#ifndef __STEKIN_INSTANCE_VARIABLE_H__ 
#define __STEKIN_INSTANCE_VARIABLE_H__ 

#include <vector>

#include "fwd-decl.h"
#include "../misc/pos-type.h"
#include "../util/pointer.h"

namespace inst {

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

        util::sptr<inst::Expression const> call(
                                        misc::position const& call_pos
                                      , std::vector<util::sref<inst::Type const>> const& arg_types
                                      , std::vector<util::sptr<Expression const>> args) const;

        Variable adjustLocation(int offset_diff, int lvl) const;

        bool operator<(Variable const& rhs) const;
        bool operator==(Variable const& rhs) const;
        bool operator!=(Variable const& rhs) const;
    };

}

#endif /* __STEKIN_INSTANCE_VARIABLE_H__ */
