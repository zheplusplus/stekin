#ifndef __STEKIN_PROTO_LIST_TYPES_H__
#define __STEKIN_PROTO_LIST_TYPES_H__

#include "type.h"

namespace proto {

    struct ListType
        : public Type
    {
        static util::sref<Type const> getListType(util::sref<Type const> member_type);
        static util::sref<Type const> getEmptyListType();
        static util::sref<Type const> memberTypeOrNulIfNotList(util::sref<Type const> list_type);
        static util::sref<Type const> commonListTypeOrNulIfImcompatible(
                                            util::sref<Type const> a, util::sref<Type const> b);
        static bool isListType(util::sref<Type const> type);
    public:
        util::sref<Type const> const member_type;
    public:
        util::sptr<inst::Type const> makeInstType() const;
        std::string name() const;
        std::vector<int> resEntries(int stack_offset) const;
        util::sref<Type const> memberCallType(std::string const& call_name
                                            , std::vector<util::sref<Type const>> const& arg_types
                                            , misc::trace& trace) const;
    private:
        explicit ListType(util::sref<Type const> mt);
    };

}

#endif /* __STEKIN_PROTO_LIST_TYPES_H__ */
