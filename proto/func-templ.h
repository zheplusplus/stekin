#ifndef __STACKENING_PROTO_FUNCTION_TEMPLATE_H__
#define __STACKENING_PROTO_FUNCTION_TEMPLATE_H__

#include <list>
#include <map>

#include "stmt-nodes.h"
#include "symbol-table.h"
#include "scope.h"
#include "../instance/function.h"
#include "../instance/type.h"
#include "../misc/pos-type.h"

namespace proto {

    struct func_templ {
        util::sref<inst::function> inst(misc::pos_type const& pos
                                      , util::sref<inst::scope> ext_scope
                                      , std::vector<inst::type const*> const& arg_types);

        func_templ(misc::pos_type const& ps, std::string const& name, std::vector<std::string> const& params);

        func_templ(misc::pos_type const& ps
                 , std::string const& func_name
                 , std::vector<std::string> const& params
                 , util::sref<symbol_table const> container_symbols);

        func_templ(func_templ&& rhs);

        func_templ(func_templ const&) = delete;

        misc::pos_type const pos;
        std::string const name;
        std::vector<std::string> const param_names;

        util::sref<scope> get_scope();
    private:
        void _fill_param_names();

        symbol_table _symbols;
        func_scope _body_scope;
    private:
        struct instance_info {
            std::map<std::string, inst::variable const> const ext_vars;
            std::vector<inst::type const*> const arg_types;

            instance_info(std::map<std::string, inst::variable const> const& e
                        , std::vector<inst::type const*> const& a)
                : ext_vars(e)
                , arg_types(a)
            {}

            bool operator<(instance_info const& rhs) const;
        };

        std::map<instance_info, util::sref<inst::function>> _instance_cache;
    private:
        std::map<std::string, inst::variable const>
                _bind_external_vars(misc::pos_type const& pos, util::sref<inst::scope const> ext_scope) const;
    };

}

#endif /* __STACKENING_PROTO_FUNCTION_TEMPLATE_H__ */
