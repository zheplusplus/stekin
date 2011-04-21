#ifndef __STACKENGING_INSTANCE_TYPE_H__
#define __STACKENGING_INSTANCE_TYPE_H__

#include <string>
#include <map>

#include "variable.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"
#include "../proto/fwd-decl.h"

namespace inst {

    struct type {
        int const size;
    public:
        virtual std::string exported_name() const = 0;
        virtual std::string name() const = 0;
    public:
        virtual util::sptr<inst::Expression const> call_func(
                  misc::pos_type const& call_pos
                , int level
                , int stack_offset
                , std::vector<util::sref<inst::type const>> const& arg_types
                , std::vector<util::sptr<Expression const>> args) const = 0;
    public:
        bool operator!=(type const& rhs) const;
        virtual bool operator==(type const& rhs) const = 0;
        virtual bool operator<(type const& rhs) const = 0;
    public:
        virtual bool eq_as_built_in(type const& lhs) const;
        virtual bool eq_as_func_reference(util::sref<proto::Function> lhs_func
                                        , std::map<std::string, variable const> const& rhs_cr) const;

        virtual bool lt_as_built_in(type const& lhs) const = 0;
        virtual bool lt_as_func_reference(util::sref<proto::Function> lhs_func
                                        , std::map<std::string, variable const> const& rhs_cr) const = 0;
    public:
        virtual void check_condition_type(misc::pos_type const& pos) const;
    protected:
        explicit type(int s)
            : size(s)
        {}
    public:
        static util::sref<type const> const BAD_TYPE;
        static util::sref<type const> const BIT_VOID;
        static util::sref<type const> const BIT_BOOL;
        static util::sref<type const> const BIT_INT;
        static util::sref<type const> const BIT_FLOAT;
    };

    struct built_in_primitive
        : public type
    {
        built_in_primitive(std::string const& n, int size)
            : type(size)
            , tname(n)
        {}

        std::string const tname;
    public:
        std::string exported_name() const;
        std::string name() const;
    public:
        util::sptr<inst::Expression const> call_func(
                  misc::pos_type const& call_pos
                , int
                , int
                , std::vector<util::sref<inst::type const>> const&
                , std::vector<util::sptr<Expression const>>) const;
    public:
        bool operator==(type const& rhs) const;
        bool operator<(type const& rhs) const;
        bool eq_as_built_in(type const& lhs) const;
        bool lt_as_built_in(type const& lhs) const;
        bool lt_as_func_reference(util::sref<proto::Function>
                                , std::map<std::string, variable const> const&) const;
    public:
        void check_condition_type(misc::pos_type const& pos) const;
    };

}

#endif /* __STACKENGING_INSTANCE_TYPE_H__ */
