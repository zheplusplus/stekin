#ifndef __STEKIN_INSTANCE_SYMBOL_TABLE_H__ 
#define __STEKIN_INSTANCE_SYMBOL_TABLE_H__

#include <string>
#include <map>
#include <list>

#include "fwd-decl.h"
#include "../misc/pos-type.h"
#include "../util/pointer.h"

namespace inst {

    struct ArgNameTypeRec {
        std::string const name;
        util::sref<Type const> const atype;

        ArgNameTypeRec(std::string const& n, util::sref<Type const> const t)
            : name(n)
            , atype(t)
        {}
    };

    struct SymbolTable {
        SymbolTable(int ext_lvl
                  , std::list<ArgNameTypeRec> const& args
                  , std::map<std::string, Variable const> const& ext_vars);

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
        Variable defVar(misc::position const& pos
                      , util::sref<Type const> type
                      , std::string const& name);
        Variable queryVar(misc::position const& pos, std::string const& name) const;

        Operation const* queryBinary(misc::position const& pos
                                   , std::string const& op
                                   , util::sref<Type const> lhs
                                   , util::sref<Type const> rhs) const;
        Operation const* queryPreUnary(misc::position const& pos
                                     , std::string const& op
                                     , util::sref<Type const> rhs) const;
    public:
        int const level;
    public:
        int stackSize() const;
        std::list<Variable> getArgs() const;
    private:
        int _ss_used;
        std::list<Variable> _args;
        std::map<std::string, Variable const> _local_defs;
        std::map<std::string, Variable const> const _external_defs;

        SymbolTable(SymbolTable const&) = delete;
    };

}

#endif /* __STEKIN_INSTANCE_SYMBOL_TABLE_H__ */
