#ifndef __STACKENING_PROTO_FUNC_PROTOTYPE_H__
#define __STACKENING_PROTO_FUNC_PROTOTYPE_H__

#include <list>
#include <map>

#include "func-instance.h"
#include "stmt-prototypes.h"
#include "func-prototype-factories.h"
#include "../symtab/prototype-space.h"

namespace proto {

    struct func_prototype {
        func_instance const* inst(int lineno
                                , symtab::inst_space const* ext_space
                                , std::vector<symtab::type const*> const& arg_types);

        func_prototype(int lineno, std::string const& func_name, std::vector<std::string> const& ps);

        func_prototype(int lineno
                     , std::string const& func_name
                     , std::vector<std::string> const& ps
                     , symtab::prototype_space const* container_space);

        func_prototype(func_prototype&& rhs);

        func_prototype(func_prototype const&) = delete;

        int const line_num;
        std::string const name;
        std::vector<std::string> const params;
    private:
        void _fill_params(std::vector<std::string> const& param_names);

        symtab::prototype_space _space;
        block_prototype _block;
        func_prototype_factory _factory;
    public:
        prototype_scope const scope;
    private:
        struct ret_info {
            ret_info(int lineno, expr_prototype const* r)
                : line_num(lineno)
                , ret_val(r)
            {}

            int const line_num;
            expr_prototype const* const ret_val;
        };

        std::list<ret_info> _returns;
        std::list<int> _void_returns;

        struct instance_info {
            std::map<std::string, symtab::variable const> const ext_vars;
            std::vector<symtab::type const*> const arg_types;

            instance_info(std::map<std::string, symtab::variable const> const& e
                        , std::vector<symtab::type const*> const& a)
                : ext_vars(e)
                , arg_types(a)
            {}

            bool operator<(instance_info const& rhs) const;
        };

        std::map<instance_info, func_instance const*> _instance_cache;
    private:
        std::map<std::string, symtab::variable const>
                        _bind_external_vars(int lineno, symtab::inst_space const* ext_space) const;
    };

}

#endif /* __STACKENING_PROTO_FUNC_PROTOTYPE_H__ */
