#ifndef __STACKENGING_INSTANCE_FUNCTION_REFERENCE_TYPE_H__
#define __STACKENGING_INSTANCE_FUNCTION_REFERENCE_TYPE_H__

#include "type.h"
#include "../misc/pos-type.h"

namespace inst {

    struct func_reference_type
        : public type
    {
        func_reference_type(misc::pos_type const& reference_pos
                          , util::sref<proto::function> func_proto
                          , int level
                          , std::map<std::string, variable const> const& cr)
            : type(_calc_size(cr))
            , context_references(cr)
            , closed_references(std::move(_enclose_reference(reference_pos, level, cr)))
            , _func_proto(func_proto)
        {}
    public:
        std::string name() const;
    public:
        util::sptr<inst::expr_base const> call_func(int level
                                                  , int stack_offset
                                                  , std::vector<util::sref<inst::type const>> const& arg_types
                                                  , std::vector<util::sptr<expr_base const>> args) const;

        bool operator==(type const& rhs) const;
        bool operator<(type const& rhs) const;
        bool eq_as_func_reference(util::sref<proto::function> lhs_func
                                , std::map<std::string, variable const> const& rhs_cr) const;
        bool lt_as_func_reference(util::sref<proto::function> lhs_func
                                , std::map<std::string, variable const> const& rhs_cr) const;
        bool lt_as_built_in(type const&) const;

        util::sref<proto::function> get_func_proto() const;

        std::map<std::string, variable const> const context_references;
        std::map<std::string, variable const> const closed_references;
    private:
        util::sref<proto::function> const _func_proto;
    private:
        static std::map<std::string, variable const> _enclose_reference(
                        misc::pos_type const& pos
                      , int level
                      , std::map<std::string, variable const> const& cr);
        static int _calc_size(std::map<std::string, variable const> const& cr);

        std::map<std::string, variable const> _adjust_offset(int stack_offset) const;
    };

}

#endif /* __STACKENGING_INSTANCE_FUNCTION_REFERENCE_TYPE_H__ */
