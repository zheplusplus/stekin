#ifndef __STEKIN_INSTANCE_TYPES_H__
#define __STEKIN_INSTANCE_TYPES_H__

#include <string>
#include <vector>

#include "../util/pointer.h"

namespace inst {

    struct Type {
        virtual ~Type() {}
    public:
        virtual std::string exportedName() const = 0;
    };

    struct VoidPrimitive
        : public Type
    {
        std::string exportedName() const;
    };

    struct IntPrimitive
        : public Type
    {
        std::string exportedName() const;
    };

    struct FloatPrimitive
        : public Type
    {
        std::string exportedName() const;
    };

    struct BoolPrimitive
        : public Type
    {
        std::string exportedName() const;
    };

    struct ClosureType
        : public Type
    {
        ClosureType(int s, std::vector<util::sptr<Type const>> e)
            : size(s)
            , enclosed_types(std::move(e))
        {}

        std::string exportedName() const;

        int size;
        std::vector<util::sptr<Type const>> enclosed_types;
    };

}

#endif /* __STEKIN_INSTANCE_TYPES_H__ */
