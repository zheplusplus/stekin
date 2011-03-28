#ifndef __STACKENING_INSTANCE_VARIABLE_H__ 
#define __STACKENING_INSTANCE_VARIABLE_H__ 

#include "fwd-decl.h"
#include "../misc/pos-type.h"
#include "../util/pointer.h"

namespace inst {

    struct variable_level {
        virtual void write(int offset, std::string const& name) const = 0;
        virtual util::sptr<variable_level const> copy() const = 0;
    };

    struct static_level
        : public variable_level
    {
        explicit static_level(int l)
            : level(l)
        {}

        void write(int offset, std::string const& name) const;
        util::sptr<variable_level const> copy() const;

        int const level;
    };

    struct current_level
        : public variable_level
    {
        void write(int offset, std::string const& name) const;
        util::sptr<variable_level const> copy() const;
    };

    struct variable {
        misc::pos_type const def_pos;
        util::sref<type const> const vtype;
        int const stack_offset;
        util::sptr<variable_level const> const level;

        variable(misc::pos_type const& pos, util::sref<type const> t, int offset, int lvl)
            : def_pos(pos)
            , vtype(t)
            , stack_offset(offset)
            , level(new static_level(lvl))
        {}

        variable(misc::pos_type const& pos, util::sref<type const> t, int offset)
            : def_pos(pos)
            , vtype(t)
            , stack_offset(offset)
            , level(new current_level)
        {}

        variable(variable const& rhs)
            : def_pos(rhs.def_pos)
            , vtype(rhs.vtype)
            , stack_offset(rhs.stack_offset)
            , level(std::move(rhs.level->copy()))
        {}

        bool operator<(variable const& rhs) const;
        bool operator==(variable const& rhs) const;
        bool operator!=(variable const& rhs) const;
    };

}

#endif /* __STACKENING_INSTANCE_VARIABLE_H__ */
