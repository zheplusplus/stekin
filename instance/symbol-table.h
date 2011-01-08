#ifndef __STACKENING_INSTANCE_SYMBOL_TABLE_H__ 
#define __STACKENING_INSTANCE_SYMBOL_TABLE_H__

#include <string>
#include <map>
#include <vector>

#include "variable.h"
#include "type.h"
#include "../misc/pos-type.h"

namespace inst {

    struct arg_name_type_pair {
        std::string const name;
        type const* const atype;

        arg_name_type_pair(std::string const& n, type const* const t)
            : name(n)
            , atype(t)
        {}
    };

    struct symbol_table {
        symbol_table(symbol_table const* ext_space
                   , std::vector<arg_name_type_pair> const& args
                   , std::map<std::string, variable const> ext_vars);

        symbol_table()
            : level(0)
            , _ss_used(0)
        {}

        symbol_table(symbol_table&& rhs)
            : level(rhs.level)
            , _ss_used(0)
            , _external_defs(std::move(rhs._external_defs))
        {}
    public:
        variable def_var(misc::pos_type const& pos, type const* vtype, std::string const& name);
        variable query_var(misc::pos_type const& pos, std::string const& name) const;
    public:
        int const level;
    private:
        int _ss_used;
        std::map<std::string, variable const> _local_defs;
        std::map<std::string, variable const> const _external_defs;

        symbol_table(symbol_table const&) = delete;
    };

}

#endif /* __STACKENING_INSTANCE_SYMBOL_TABLE_H__ */
