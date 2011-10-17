#ifndef __STEKIN_PROTO_FUNCTION_REFERENCE_TYPE_H__
#define __STEKIN_PROTO_FUNCTION_REFERENCE_TYPE_H__

#include <instance/expr-nodes.h>
#include <misc/pos-type.h>

#include "type.h"
#include "variable.h"

namespace proto {

    struct FuncReferenceType
        : public Type
    {
        FuncReferenceType(misc::position const& reference_pos
                        , util::sref<Function> func
                        , int level
                        , std::map<std::string, Variable const> const& cr)
            : Type(_calcSize(cr))
            , context_references(cr)
            , closed_references(std::move(_encloseReference(reference_pos, level, cr)))
            , _func(func)
        {}
    public:
        util::sptr<inst::Type const> makeInstType() const;
        std::string name() const;
    public:
        util::sref<FuncInstDraft> call(int level
                                     , int stack_offset
                                     , std::vector<util::sref<Type const>> const& arg_types
                                     , misc::trace& trace) const;
    public:
        std::map<std::string, Variable const> const context_references;
        std::map<std::string, Variable const> const closed_references;
    public:
        std::list<inst::FuncReference::ArgInfo> makeCallArgs() const;
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
