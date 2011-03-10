#ifndef __STACKENING_INSTANCE_INSTANTIATE_MEDIATE_H__
#define __STACKENING_INSTANCE_INSTANTIATE_MEDIATE_H__

#include "fwd-decl.h"
#include "../util/pointer.h"

namespace inst {

    struct mediate_base {
        virtual ~mediate_base() {}

        virtual util::sptr<stmt_base const> inst(util::sref<scope> sc) = 0;
        virtual void mediate_inst(util::sref<scope> sc) = 0;
    protected:
        mediate_base() {}
    };

}

#endif /* __STACKENING_INSTANCE_INSTANTIATE_MEDIATE_H__ */
