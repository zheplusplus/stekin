#ifndef __STACKENGING_INSTANCE_FUNCTION_REFERENCE_TYPE_H__
#define __STACKENGING_INSTANCE_FUNCTION_REFERENCE_TYPE_H__

#include "type.h"
#include "../misc/pos-type.h"

namespace inst {

    struct FuncReferenceType
        : public Type
    {
        FuncReferenceType(misc::position const& reference_pos
                        , util::sref<proto::Function> func_proto
                        , int level
                        , std::map<std::string, Variable const> const& cr)
            : Type(_calcSize(cr))
            , context_references(cr)
            , closed_references(std::move(_encloseReference(reference_pos, level, cr)))
            , _func_proto(func_proto)
        {}
    public:
        std::string exportedName() const;
        std::string name() const;
    public:
        util::sptr<inst::Expression const> call(misc::position const&
                                              , int level
                                              , int stack_offset
                                              , std::vector<util::sref<Type const>> const& arg_types
                                              , std::vector<util::sptr<Expression const>> args)
                                            const;

        bool operator==(Type const& rhs) const;
        bool operator<(Type const& rhs) const;
        bool eqAsFuncReference(util::sref<proto::Function> lhs_func
                             , std::map<std::string, Variable const> const& rhs_cr) const;
        bool ltAsFuncReference(util::sref<proto::Function> lhs_func
                             , std::map<std::string, Variable const> const& rhs_cr) const;
        bool ltAsBuiltIn(Type const&) const;
    public:
        std::map<std::string, Variable const> const context_references;
        std::map<std::string, Variable const> const closed_references;
    public:
        void write() const;
    private:
        util::sref<proto::Function> const _func_proto;
    private:
        static std::map<std::string, Variable const> _encloseReference(
                        misc::position const& pos
                      , int level
                      , std::map<std::string, Variable const> const& cr);
        static int _calcSize(std::map<std::string, Variable const> const& cr);

        std::map<std::string, Variable const> _adjustVars(int stack_offset, int level) const;
    };

}

#endif /* __STACKENGING_INSTANCE_FUNCTION_REFERENCE_TYPE_H__ */
