#ifndef __STACKENING_INSTANCE_VARIABLE_H__ 
#define __STACKENING_INSTANCE_VARIABLE_H__ 

#include "type.h"
#include "../misc/pos-type.h"

namespace inst {

    struct variable {
        misc::pos_type const def_pos;
        type const* const vtype;
        int const stack_offset;
        int const level;

        variable(misc::pos_type const& pos, type const* t, int offset, int lvl)
            : def_pos(pos)
            , vtype(t)
            , stack_offset(offset)
            , level(lvl)
        {}

        bool operator<(variable const& rhs) const
        {
            if (vtype != rhs.vtype) {
                return vtype < rhs.vtype;
            }
            if (stack_offset != rhs.stack_offset) {
                return stack_offset < rhs.stack_offset;
            }
            return level < rhs.level;
        }

        bool operator==(variable const& rhs) const
        {
            return vtype == rhs.vtype && stack_offset == rhs.stack_offset && level == rhs.level;
        }

        bool operator!=(variable const& rhs) const
        {
            return !operator==(rhs);
        }
    };

}

#endif /* __STACKENING_INSTANCE_VARIABLE_H__ */
