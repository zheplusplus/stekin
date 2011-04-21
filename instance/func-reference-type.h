#ifndef __STACKENGING_INSTANCE_FUNCTION_REFERENCE_TYPE_H__
#define __STACKENGING_INSTANCE_FUNCTION_REFERENCE_TYPE_H__

#include "type.h"
#include "../misc/pos-type.h"

namespace inst {

    struct func_reference_type
        : public type
    {
        func_reference_type(misc::pos_type const& reference_pos
                          , util::sref<proto::Function> func_proto
                          , int level
                          , std::map<std::string, variable const> const& cr)
            : type(_calc_size(cr))
            , context_references(cr)
            , closed_references(std::move(_enclose_reference(reference_pos, level, cr)))
            , _func_proto(func_proto)
        {}
    public:
        std::string exported_name() const;
        std::string name() const;
    public:
        util::sptr<inst::Expression const> call_func(misc::pos_type const&
                                                  , int level
                                                  , int stack_offset
                                                  , std::vector<util::sref<inst::type const>> const& arg_types
                                                  , std::vector<util::sptr<Expression const>> args) const;

        bool operator==(type const& rhs) const;
        bool operator<(type const& rhs) const;
        bool eq_as_func_reference(util::sref<proto::Function> lhs_func
                                , std::map<std::string, variable const> const& rhs_cr) const;
        bool lt_as_func_reference(util::sref<proto::Function> lhs_func
                                , std::map<std::string, variable const> const& rhs_cr) const;
        bool lt_as_built_in(type const&) const;
    public:
        std::map<std::string, variable const> const context_references;
        std::map<std::string, variable const> const closed_references;
    public:
        void write() const;
    private:
        util::sref<proto::Function> const _func_proto;
    private:
        static std::map<std::string, variable const> _enclose_reference(
                        misc::pos_type const& pos
                      , int level
                      , std::map<std::string, variable const> const& cr);
        static int _calc_size(std::map<std::string, variable const> const& cr);

        std::map<std::string, variable const> _adjust_vars(int stack_offset, int level) const;
    };

}

#endif /* __STACKENGING_INSTANCE_FUNCTION_REFERENCE_TYPE_H__ */
