#ifndef __STEKIN_PROTO_FUNCTION_TEMPLATE_H__
#define __STEKIN_PROTO_FUNCTION_TEMPLATE_H__

#include <list>
#include <map>

#include "general-scope.h"
#include "../instance/fwd-decl.h"
#include "../instance/variable.h"
#include "../misc/pos-type.h"

namespace proto {

    struct Function
        : public GeneralScope
    {
        util::sref<inst::Function> inst(misc::position const& pos
                                      , util::sref<inst::Scope> ext_scope
                                      , std::vector<util::sref<inst::type const>> const& arg_types);
        util::sref<inst::Function> inst(int level
                                      , std::map<std::string, inst::variable const> const& ext_vars
                                      , std::vector<util::sref<inst::type const>> const& arg_types);

        Function(misc::position const& ps
               , std::string const& func_name
               , std::vector<std::string> const& params
               , util::sref<SymbolTable const> ext_symbols
               , bool func_hint_void_return);

        Function(Function&& rhs)
            : GeneralScope(std::move(rhs))
            , pos(rhs.pos)
            , name(rhs.name)
            , param_names(rhs.param_names)
            , hint_void_return(rhs.hint_void_return)
        {}

        Function(Function const&) = delete;

        misc::position const pos;
        std::string const name;
        std::vector<std::string> const param_names;
        bool hint_void_return;
    private:
        void _fill_param_names();
    private:
        struct instance_info {
            std::map<std::string, inst::variable const> const ext_vars;
            std::vector<util::sref<inst::type const>> const arg_types;

            instance_info(std::map<std::string, inst::variable const> const& e
                        , std::vector<util::sref<inst::type const>> const& a)
                : ext_vars(e)
                , arg_types(a)
            {}

            bool operator<(instance_info const& rhs) const;
        };

        std::map<instance_info, util::sref<inst::Function>> _instance_cache;
    public:
        std::map<std::string, inst::variable const>
                bind_external_vars(misc::position const& pos, util::sref<inst::Scope const> ext_scope) const;
    };

}

#endif /* __STEKIN_PROTO_FUNCTION_TEMPLATE_H__ */
