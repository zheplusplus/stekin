#ifndef __STACKENGING_INSTANCE_FUNCTION_REFERENCE_TYPE_H__
#define __STACKENGING_INSTANCE_FUNCTION_REFERENCE_TYPE_H__

#include "type.h"

namespace inst {

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
        bool lt_as_built_in(type const&) const;

        util::sref<proto::function> get_func_proto() const;

        std::map<std::string, variable const> const context_references;
    private:
        util::sref<proto::function> const _func_proto;
    };

}

#endif /* __STACKENGING_INSTANCE_FUNCTION_REFERENCE_TYPE_H__ */
