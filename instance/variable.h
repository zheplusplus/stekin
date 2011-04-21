#ifndef __STEKIN_INSTANCE_VARIABLE_H__ 
#define __STEKIN_INSTANCE_VARIABLE_H__ 

#include <vector>

#include "fwd-decl.h"
#include "../misc/pos-type.h"
#include "../util/pointer.h"

namespace inst {

    struct variable {
        misc::position const def_pos;
        util::sref<type const> const vtype;
        int const stack_offset;
        int const level;

        variable(misc::position const& pos, util::sref<type const> t, int offset, int lvl)
            : def_pos(pos)
            , vtype(t)
            , stack_offset(offset)
            , level(lvl)
        {}

        util::sptr<inst::Expression const> call_func(misc::position const& call_pos
                                                  , std::vector<util::sref<inst::type const>> const& arg_types
                                                  , std::vector<util::sptr<Expression const>> args) const;

        variable adjust_location(int offset_diff, int lvl) const;

        bool operator<(variable const& rhs) const;
        bool operator==(variable const& rhs) const;
        bool operator!=(variable const& rhs) const;
    };

}

#endif /* __STEKIN_INSTANCE_VARIABLE_H__ */
