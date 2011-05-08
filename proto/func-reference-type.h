#ifndef __STEKIN_PROTO_FUNCTION_REFERENCE_TYPE_H__
#define __STEKIN_PROTO_FUNCTION_REFERENCE_TYPE_H__

#include "../instance/expr-nodes.h"
#include "../instance/type.h"
#include "../misc/pos-type.h"

namespace proto {

    struct FuncReferenceType
        : public inst::Type
    {
        FuncReferenceType(misc::position const& reference_pos
                        , util::sref<Function> func
                        , int level
                        , std::map<std::string, Variable const> const& cr)
            : inst::Type(_calcSize(cr))
            , context_references(cr)
            , closed_references(std::move(_encloseReference(reference_pos, level, cr)))
            , _func(func)
        {}
    public:
        std::string exportedName() const;
        std::string name() const;
    public:
        util::sptr<inst::Expression const> call(
                                    misc::position const&
                                  , int level
                                  , int stack_offset
                                  , std::vector<util::sref<inst::Type const>> const& arg_types
                                  , std::vector<util::sptr<inst::Expression const>> args) const;

        bool operator==(inst::Type const& rhs) const;
        bool operator<(inst::Type const& rhs) const;
        bool eqAsFuncReference(util::sref<Function> lhs_func
                             , std::map<std::string, Variable const> const& rhs_cr) const;
        bool ltAsFuncReference(util::sref<Function> lhs_func
                             , std::map<std::string, Variable const> const& rhs_cr) const;
        bool ltAsBuiltIn(inst::Type const&) const;
    public:
        std::map<std::string, Variable const> const context_references;
        std::map<std::string, Variable const> const closed_references;
    public:
        std::vector<inst::FuncReference::ArgInfo> makeCallArgs() const;
    private:
        util::sref<Function> const _func;
    private:
        static std::map<std::string, Variable const> _encloseReference(
                        misc::position const& pos
                      , int level
                      , std::map<std::string, Variable const> const& cr);
        static int _calcSize(std::map<std::string, Variable const> const& cr);

        std::map<std::string, Variable const> _adjustVars(int stack_offset, int level) const;
    };

}

#endif /* __STEKIN_PROTO_FUNCTION_REFERENCE_TYPE_H__ */
