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

        virtual std::string name() const = 0;

        bool operator!=(type const& rhs) const;
        virtual bool operator==(type const& rhs) const = 0;
        virtual bool operator<(type const& rhs) const = 0;
    public:
        virtual bool eq_as_built_in(type const& lhs) const;
        virtual bool eq_as_func_reference(util::sref<proto::function> lhs_func
                                        , std::map<std::string, variable const> const& rhs_cr) const;

        virtual bool lt_as_built_in(type const& lhs) const;
        virtual bool lt_as_func_reference(util::sref<proto::function> lhs_func
                                        , std::map<std::string, variable const> const& rhs_cr) const;
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

        std::string name() const;

        bool operator==(type const& rhs) const;
        bool operator<(type const& rhs) const;
        bool eq_as_built_in(type const& lhs) const;
        bool lt_as_built_in(type const& lhs) const;
    };

    struct func_reference_type
        : public type
    {
        func_reference_type(util::sref<proto::function> func_proto
                          , std::map<std::string, variable const> const& cr)
            : type(0)
            , context_references(cr)
            , _func_proto(func_proto)
        {}

        std::string name() const;

        bool operator==(type const& rhs) const;
        bool operator<(type const& rhs) const;
        bool eq_as_func_reference(util::sref<proto::function> lhs_func
                                , std::map<std::string, variable const> const& rhs_cr) const;
        bool lt_as_func_reference(util::sref<proto::function> lhs_func
                                , std::map<std::string, variable const> const& rhs_cr) const;

        util::sref<proto::function> get_func_proto() const;

        std::map<std::string, variable const> const context_references;
    private:
        util::sref<proto::function> const _func_proto;
    };

    void check_condition_type(misc::pos_type const& pos, util::sref<type const> t);

}

#endif /* __STACKENGING_INSTANCE_TYPE_H__ */
