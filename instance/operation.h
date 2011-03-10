#ifndef __STACKENGING_INSTANCE_OPERATION_H__
#define __STACKENGING_INSTANCE_OPERATION_H__

#include <string>

#include "fwd-decl.h"
#include "../misc/pos-type.h"

namespace inst {

    struct operation {
        type const* const ret_type;
        std::string const op_img;

        operation(type const* rt, std::string const& oi)
            : ret_type(rt)
            , op_img(oi)
        {}

        static operation const* query_binary(misc::pos_type const& pos
                                           , std::string const& op
                                           , type const* lhs
                                           , type const* rhs);
        static operation const* query_pre_unary(misc::pos_type const& pos
                                              , std::string const& op
                                              , type const* rhs);
    };

}

#endif /* __STACKENGING_INSTANCE_OPERATION_H__ */
