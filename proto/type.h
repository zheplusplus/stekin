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
        virtual bool operator==(Type const& rhs) const = 0;
        virtual bool operator<(Type const& rhs) const = 0;
    public:
        virtual bool eqAsBuiltIn(Type const& lhs) const;
        virtual bool eqAsFuncReference(util::sref<Function> lhs_func
                                     , std::map<std::string, Variable const> const& rhs_cr) const;

        virtual bool ltAsBuiltIn(Type const& lhs) const = 0;
        virtual bool ltAsFuncReference(util::sref<Function> lhs_func
                                     , std::map<std::string, Variable const> const& rhs_cr)
                                    const = 0;
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

    struct BuiltInPrimitive
        : public Type
    {
        BuiltInPrimitive(std::string const& n, int size)
            : Type(size)
            , tname(n)
        {}

        std::string const tname;
    public:
        std::string name() const;
    public:
        util::sref<FuncInstDraft> call(misc::position const& call_pos
                                     , int
                                     , int
                                     , std::vector<util::sref<Type const>> const&) const;
    public:
        bool operator==(Type const& rhs) const;
        bool operator<(Type const& rhs) const;
        bool eqAsBuiltIn(Type const& lhs) const;
        bool ltAsBuiltIn(Type const& lhs) const;
        bool ltAsFuncReference(util::sref<Function>
                             , std::map<std::string, Variable const> const&) const;
    public:
        void checkCondType(misc::position const& pos) const;
    };

}

#endif /* __STEKIN_PROTO_TYPE_H__ */
