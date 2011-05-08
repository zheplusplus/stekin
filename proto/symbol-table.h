#ifndef __STEKIN_PROTO_SYMBOL_TABLE_H__ 
#define __STEKIN_PROTO_SYMBOL_TABLE_H__

#include <string>
#include <map>
#include <list>

#include "fwd-decl.h"
#include "../instance/fwd-decl.h"
#include "../misc/pos-type.h"
#include "../util/pointer.h"

namespace proto {

    struct ArgNameTypeRec {
        std::string const name;
        util::sref<inst::Type const> const atype;

        ArgNameTypeRec(std::string const& n, util::sref<inst::Type const> const t)
            : name(n)
            , atype(t)
        {}
    };

    struct SymbolTable {
        SymbolTable(int ext_lvl
                  , std::list<ArgNameTypeRec> const& args
                  , std::map<std::string, inst::Variable const> const& ext_vars);

        SymbolTable()
            : level(0)
            , _ss_used(0)
        {}
    public:
        inst::Variable defVar(misc::position const& pos
                            , util::sref<inst::Type const> type
                            , std::string const& name);
        inst::Variable queryVar(misc::position const& pos, std::string const& name) const;

        Operation const* queryBinary(misc::position const& pos
                                   , std::string const& op
                                   , util::sref<inst::Type const> lhs
                                   , util::sref<inst::Type const> rhs) const;
        Operation const* queryPreUnary(misc::position const& pos
                                     , std::string const& op
                                     , util::sref<inst::Type const> rhs) const;
    public:
        int const level;
    public:
        int stackSize() const;
        std::list<inst::Variable> getArgs() const;
    private:
        int _ss_used;
        std::list<inst::Variable> _args;
        std::map<std::string, inst::Variable const> _local_defs;
        std::map<std::string, inst::Variable const> const _external_defs;

        SymbolTable(SymbolTable const&) = delete;
    };

}

#endif /* __STEKIN_PROTO_SYMBOL_TABLE_H__ */
