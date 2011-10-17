#ifndef __STEKIN_PROTO_OPERATION_H__
#define __STEKIN_PROTO_OPERATION_H__

#include <string>

#include <util/pointer.h>
#include <misc/pos-type.h>

#include "fwd-decl.h"

namespace proto {

    struct Operation {
        util::sref<Type const> const ret_type;
        std::string const op_img;

        Operation(util::sref<Type const> rt, std::string const& oi)
            : ret_type(rt)
            , op_img(oi)
        {}

        static util::sref<Operation const> queryBinary(misc::position const& pos
                                                     , std::string const& op
                                                     , util::sref<Type const> lhs
                                                     , util::sref<Type const> rhs);
        static util::sref<Operation const> queryPreUnary(misc::position const& pos
                                                       , std::string const& op
                                                       , util::sref<Type const> rhs);
    };

}

#endif /* __STEKIN_PROTO_OPERATION_H__ */
