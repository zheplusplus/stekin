#ifndef __STEKIN_INSTANCE_BUILT_IN_H__
#define __STEKIN_INSTANCE_BUILT_IN_H__

#include <vector>

#include <util/pointer.h>

#include "node-base.h"

namespace inst {

    struct WriterExpr
        : public Expression
    {
        explicit WriterExpr(util::sptr<inst::Expression const> e)
            : expr(std::move(e))
        {}

        void write() const;

        util::sptr<Expression const> const expr;
    };

}

#endif /* __STEKIN_INSTANCE_BUILT_IN_H__ */
