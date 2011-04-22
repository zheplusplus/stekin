#ifndef __STEKIN_INSTANCE_SYMBOL_TABLE_H__ 
#define __STEKIN_INSTANCE_SYMBOL_TABLE_H__

#include <string>
#include <map>
#include <list>

#include "fwd-decl.h"
#include "../misc/pos-type.h"
#include "../util/pointer.h"

namespace inst {

    struct arg_name_type_pair {
        std::string const name;
        util::sref<type const> const atype;

        arg_name_type_pair(std::string const& n, util::sref<type const> const t)
            : name(n)
            , atype(t)
        {}
    };

    struct SymbolTable {
        SymbolTable(int ext_lvl
                   , std::list<arg_name_type_pair> const& args
                   , std::map<std::string, variable const> const& ext_vars);

        SymbolTable()
            : level(0)
            , _ss_used(0)
        {}

        SymbolTable(SymbolTable&& rhs)
            : level(rhs.level)
            , _ss_used(0)
            , _args(rhs._args)
            , _external_defs(std::move(rhs._external_defs))
        {}
    public:
        variable defVar(misc::position const& pos, util::sref<type const> vtype, std::string const& name);
        variable query_var(misc::position const& pos, std::string const& name) const;
    public:
        int const level;
    public:
        int stack_size() const;
        std::list<variable> get_args() const;
    private:
        int _ss_used;
        std::list<variable> _args;
        std::map<std::string, variable const> _local_defs;
        std::map<std::string, variable const> const _external_defs;

        SymbolTable(SymbolTable const&) = delete;
    };

}

#endif /* __STEKIN_INSTANCE_SYMBOL_TABLE_H__ */
