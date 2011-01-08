#ifndef __STACKENGING_INSTANCE_TYPE_H__
#define __STACKENGING_INSTANCE_TYPE_H__

#include <string>

#include "../misc/pos-type.h"

namespace inst {

    struct type {
        std::string const name;
        int const size;

        type(std::string const& n, int s)
            : name(n)
            , size(s)
        {}

        static type const* const BAD_TYPE;
        static type const* const BIT_VOID;
        static type const* const BIT_BOOL;
        static type const* const BIT_INT;
        static type const* const BIT_FLOAT;
    };

    void check_condition_type(misc::pos_type const& pos, type const* t);

}

#endif /* __STACKENGING_INSTANCE_TYPE_H__ */
