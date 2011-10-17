#ifndef __STEKIN_INSTANCE_TYPES_H__
#define __STEKIN_INSTANCE_TYPES_H__

#include <string>
#include <vector>

#include <output/func-writer.h>
#include <util/pointer.h>

#include "address.h"

namespace inst {

    struct Type {
        virtual ~Type() {}
    public:
        virtual std::string exportedName() const = 0;
        virtual void writeResEntry(int offset) const;
        virtual util::sptr<output::StackVarRec const> makeParameter(Address const& addr) const;
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

    struct EmptyListType
        : public Type
    {
        std::string exportedName() const;
    };

    struct ListType
        : public Type
    {
        explicit ListType(util::sptr<Type const> mt)
            : member_type(std::move(mt))
        {}

        std::string exportedName() const;
        void writeResEntry(int offset) const;
        util::sptr<output::StackVarRec const> makeParameter(Address const& addr) const;

        util::sptr<Type const> const member_type;
    };

    struct ClosureType
        : public Type
    {
        ClosureType(int s, std::vector<util::sptr<Type const>> e)
            : size(s)
            , enclosed_types(std::move(e))
        {}

        std::string exportedName() const;

        int const size;
        std::vector<util::sptr<Type const>> const enclosed_types;
    };

}

#endif /* __STEKIN_INSTANCE_TYPES_H__ */
