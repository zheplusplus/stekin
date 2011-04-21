#ifndef __STACKENGING_INSTANCE_OPERATION_H__
#define __STACKENGING_INSTANCE_OPERATION_H__

#include <string>

#include "fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace inst {

    struct operation {
        util::sref<type const> const ret_type;
        std::string const op_img;

        operation(util::sref<type const> rt, std::string const& oi)
            : ret_type(rt)
            , op_img(oi)
        {}

        static operation const* query_binary(misc::position const& pos
                                           , std::string const& op
                                           , util::sref<type const> lhs
                                           , util::sref<type const> rhs);
        static operation const* query_pre_unary(misc::position const& pos
                                              , std::string const& op
                                              , util::sref<type const> rhs);
    };

}

#endif /* __STACKENGING_INSTANCE_OPERATION_H__ */
