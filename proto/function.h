#ifndef __STEKIN_PROTO_FUNCTION_TEMPLATE_H__
#define __STEKIN_PROTO_FUNCTION_TEMPLATE_H__

#include <list>
#include <map>

#include "fwd-decl.h"
#include "scope.h"
#include "../misc/pos-type.h"

namespace proto {

    struct Function
        : public Scope
    {
        util::sref<FuncInstDraft> inst(misc::position const& pos
                                     , util::sref<SymbolTable const> ext_st
                                     , std::vector<util::sref<Type const>> const& arg_types);
        util::sref<FuncInstDraft> inst(int level
                                     , std::map<std::string, Variable const> const& ext_vars
                                     , std::vector<util::sref<Type const>> const& arg_types);

        Function(misc::position const& ps
               , std::string const& func_name
               , std::vector<std::string> const& params
               , bool func_hint_void_return)
            : pos(ps)
            , name(func_name)
            , param_names(params)
            , hint_void_return(func_hint_void_return)
        {}

        Function(Function const&) = delete;

        misc::position const pos;
        std::string const name;
        std::vector<std::string> const param_names;
        bool hint_void_return;

        void setFreeVariables(std::vector<std::string> const& free_vars);
        std::map<std::string, Variable const> bindExternalVars(
                                                misc::position const& pos
                                              , util::sref<SymbolTable const> ext_st) const;
    public:
        struct InstanceInfo {
            std::map<std::string, Variable const> const ext_vars;
            std::vector<util::sref<Type const>> const arg_types;

            InstanceInfo(std::map<std::string, Variable const> const& e
                       , std::vector<util::sref<Type const>> const& a)
                : ext_vars(e)
                , arg_types(a)
            {}

            bool operator<(InstanceInfo const& rhs) const;
        };
    private:
        std::map<InstanceInfo, util::sref<FuncInstDraft>> _instance_cache;
        std::vector<std::string> _free_variables;
    };

}

#endif /* __STEKIN_PROTO_FUNCTION_TEMPLATE_H__ */
