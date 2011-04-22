#ifndef __STEKIN_INSTANCE_INSTANTIATE_MEDIATE_H__
#define __STEKIN_INSTANCE_INSTANTIATE_MEDIATE_H__

#include "fwd-decl.h"
#include "../util/pointer.h"

namespace inst {

    struct MediateBase {
        virtual ~MediateBase() {}

        virtual util::sptr<Statement const> inst(util::sref<Scope> sc) = 0;
        virtual void mediate_inst(util::sref<Scope> sc) = 0;
    protected:
        MediateBase() {}
    };

}

#endif /* __STEKIN_INSTANCE_INSTANTIATE_MEDIATE_H__ */
