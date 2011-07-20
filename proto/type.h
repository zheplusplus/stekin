#ifndef __STEKIN_PROTO_TYPE_H__
#define __STEKIN_PROTO_TYPE_H__

#include <string>
#include <vector>
#include <map>

#include "fwd-decl.h"
#include "../instance/types.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace proto {

    struct Type {
        int const size;
    public:
        virtual util::sptr<inst::Type const> makeInstType() const = 0;
        virtual std::string name() const = 0;
    public:
        virtual util::sref<FuncInstDraft> call(
                              misc::position const& call_pos
                            , int level
                            , int stack_offset
                            , std::vector<util::sref<Type const>> const& arg_types) const = 0;
    public:
        bool operator!=(Type const& rhs) const;
        bool operator==(Type const& rhs) const;
        bool operator<(Type const& rhs) const;
    public:
        virtual void checkCondType(misc::position const& pos) const;
    protected:
        explicit Type(int s)
            : size(s)
        {}
    public:
        static util::sref<Type const> const BAD_TYPE;
        static util::sref<Type const> const BIT_VOID;
        static util::sref<Type const> const BIT_BOOL;
        static util::sref<Type const> const BIT_INT;
        static util::sref<Type const> const BIT_FLOAT;
    };

}

#endif /* __STEKIN_PROTO_TYPE_H__ */
