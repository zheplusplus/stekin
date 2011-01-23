#ifndef __STACKENING_INSTANCE_INSTANTIATE_MEDIATE_H__
#define __STACKENING_INSTANCE_INSTANTIATE_MEDIATE_H__

#include "../util/pointer.h"

#include "node-base.h"
#include "type.h"

namespace inst {

    struct scope;

    struct mediate_base {
        virtual ~mediate_base() {}

        virtual util::sptr<stmt_base const> inst(util::sref<scope const> sc) = 0;
        virtual void mediate_inst(util::sref<scope const> sc) = 0;
    protected:
        mediate_base() {}
    };

}

#endif /* __STACKENING_INSTANCE_INSTANTIATE_MEDIATE_H__ */
