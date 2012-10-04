#ifndef __STEKIN_PROTO_TYPE_H__
#define __STEKIN_PROTO_TYPE_H__

#include <string>
#include <vector>
#include <map>

#include <instance/types.h>
#include <util/pointer.h>
#include <misc/pos-type.h>

#include "fwd-decl.h"

namespace proto {

    struct Type {
        explicit Type(int s)
            : size(s)
        {}
    public:
        int const size;
    public:
        virtual util::sptr<inst::Type const> makeInstType() const = 0;
        virtual std::string name() const = 0;
    public:
        virtual util::sref<FuncInstDraft> call(int level
                                             , int stack_offset
                                             , std::vector<util::sref<Type const>> const& arg_types
                                             , misc::trace& trace) const;
        virtual void checkCondType(misc::position const& pos) const;
        virtual std::vector<int> resEntries(int stack_offset) const;
        virtual util::sref<Type const> memberCallType(
                                            std::string const& call_name
                                          , std::vector<util::sref<Type const>> const& arg_types
                                          , misc::trace& trace) const;
    public:
        bool operator!=(Type const& rhs) const;
        bool operator==(Type const& rhs) const;
        bool operator<(Type const& rhs) const;
    public:
        static util::sref<Type const> bad();
        static util::sref<Type const> s_void();
        static util::sref<Type const> s_bool();
        static util::sref<Type const> s_int();
        static util::sref<Type const> s_float();
        static util::sref<Type const> s_string();
    };

}

#endif /* __STEKIN_PROTO_TYPE_H__ */
