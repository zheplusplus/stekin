#include <vector>
#include <map>

#include <misc/platform.h>
#include <report/errors.h>

#include "list-types.h"

using namespace proto;

namespace {

    struct EmptyListType
        : public Type
    {
        EmptyListType()
            : Type(0)
        {}

        util::sptr<inst::Type const> makeInstType() const
        {
            return util::mkptr(new inst::EmptyListType);
        }

        std::string name() const
        {
            return "Stekin empty list";
        }

        util::sref<Type const> memberCallType(std::string const& call_name
                                            , std::vector<util::sref<Type const>> const& arg_types
                                            , misc::trace& trace) const
        {
            if ("push_back" == call_name && 1 == arg_types.size()) {
                return ListType::getListType(arg_types[0]);
            }
            if ("empty" == call_name && 0 == arg_types.size()) {
                return BIT_BOOL;
            }
            if ("size" == call_name && 0 == arg_types.size()) {
                return BIT_INT;
            }
            error::memberCallNotFound(trace.top(), name(), call_name);
            return BAD_TYPE;
        }
    };

}

static EmptyListType const empty_list_type;
static std::vector<util::sptr<ListType const>> list_type_entities;
static std::map<util::sref<Type const>, util::sref<ListType const>> type_to_list_type;
static std::map<util::sref<Type const>, util::sref<Type const>> type_to_member_type;

util::sref<Type const> ListType::getListType(util::sref<Type const> member_type)
{
    auto find_result = type_to_list_type.find(member_type);
    if (type_to_list_type.end() == find_result) {
        list_type_entities.push_back(util::mkptr(new ListType(member_type)));
        type_to_list_type.insert(std::make_pair(member_type, *list_type_entities.back()));
        type_to_member_type.insert(std::make_pair(*list_type_entities.back(), member_type));
        return *list_type_entities.back();
    }
    return find_result->second;
}

util::sref<Type const> ListType::getEmptyListType()
{
    return util::mkref(empty_list_type);
}

util::sref<Type const> ListType::memberTypeOrNulIfNotList(util::sref<Type const> type)
{
    auto find_result = type_to_member_type.find(type);
    if (type_to_member_type.end() == find_result) {
        return util::sref<Type const>(nullptr);
    }
    return find_result->second;
}

static util::sref<Type const> listTypeOrNulIfNotList(util::sref<Type const> type)
{
    if (ListType::getEmptyListType() == type) {
        return type;
    }
    if (type_to_member_type.end() == type_to_member_type.find(type)) {
        return util::sref<Type const>(nullptr);
    }
    return type;
}

util::sref<Type const> ListType::commonListTypeOrNulIfImcompatible(
                                            util::sref<Type const> a, util::sref<Type const> b)
{
    if (getEmptyListType() == a) {
        return listTypeOrNulIfNotList(b);
    }
    if (getEmptyListType() == b) {
        return listTypeOrNulIfNotList(a);
    }

    util::sref<Type const> member_type_a(memberTypeOrNulIfNotList(a));
    util::sref<Type const> member_type_b(memberTypeOrNulIfNotList(b));
    if (member_type_a.nul() || member_type_b.nul() || member_type_a != member_type_b) {
        return util::sref<Type const>(nullptr);
    }

    return a;
}

bool ListType::isListType(util::sref<Type const> type)
{
    return getEmptyListType() == type
        || type_to_member_type.end() != type_to_member_type.find(type);
}

std::string ListType::name() const
{
    return "list of [ " + member_type->name() + " ]";
}

util::sptr<inst::Type const> ListType::makeInstType() const
{
    return util::mkptr(new inst::ListType(member_type->makeInstType()));
}

std::vector<int> ListType::resEntries(int stack_offset) const
{
    std::vector<int> entry;
    entry.push_back(stack_offset);
    return entry;
}

util::sref<Type const> ListType::memberCallType(std::string const& call_name
                                              , std::vector<util::sref<Type const>> const& arg_types
                                              , misc::trace& trace) const
{
    if ("push_back" == call_name && 1 == arg_types.size() && member_type == arg_types[0]) {
        return util::mkref(*this);
    }
    if ("empty" == call_name && 0 == arg_types.size()) {
        return BIT_BOOL;
    }
    if ("size" == call_name && 0 == arg_types.size()) {
        return BIT_INT;
    }
    if ("first" == call_name && 0 == arg_types.size()) {
        return member_type;
    }
    error::memberCallNotFound(trace.top(), name(), call_name);
    return BAD_TYPE;
}

ListType::ListType(util::sref<Type const> mt)
    : Type(platform::WORD_LENGTH_INBYTE + platform::INT_SIZE + platform::VIRTUAL_FUNC_TABLE_SIZE)
    , member_type(mt)
{}
