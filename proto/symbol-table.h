#ifndef __STEKIN_PROTO_SYMBOL_TABLE_H__
#define __STEKIN_PROTO_SYMBOL_TABLE_H__

#include <string>
#include <map>
#include <list>
#include <vector>

#include "fwd-decl.h"
#include "../instance/fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace proto {

    struct Overloads {
        explicit Overloads(util::sref<Overloads const> external_overloads)
            : _external_overloads_or_nul_on_global(external_overloads)
        {}
    private:
        struct Overload {
            explicit Overload(std::string const& n)
                : name(n)
            {}

            std::string const name;
        public:
            util::sref<Function> queryOrNulIfNonexist(int param_count) const;
            void declare(util::sref<Function> func);
            std::vector<util::sref<Function>> all() const;
        private:
            std::map<int, util::sref<Function> const> _funcs;
        };
    public:
        std::vector<util::sref<Function>> allFuncsOfName(std::string const& name) const;
        util::sref<Function> queryOrNulIfNonexist(std::string const& name, int param_count) const;
        void declare(util::sref<Function> func);
    private:
        util::sref<Overload> _overloadByName(std::string const& name);
        util::sref<Overload const> _overloadByNameOrNulIfNonexist(std::string const& name) const;
    private:
        util::sref<Overloads const> const _external_overloads_or_nul_on_global;
        std::list<Overload> _overloads;
    };

    struct SymbolTable {
        int const level;

        explicit SymbolTable(util::sref<SymbolTable const> ext_symbols)
            : level(ext_symbols->level + 1)
            , _overloads(util::mkref(ext_symbols->_overloads))
        {}

        SymbolTable()
            : level(0)
            , _overloads(util::sref<Overloads const>(NULL))
        {}

        SymbolTable(SymbolTable&& rhs)
            : level(rhs.level)
            , _external_var_refs(rhs._external_var_refs)
            , _VarDefs(rhs._VarDefs)
            , _overloads(rhs._overloads)
            , _func_entities(std::move(rhs._func_entities))
        {}

        util::sptr<Expression const> refVar(misc::position const& pos, std::string const& name);
        void defVar(misc::position const& pos, std::string const& name);
        util::sref<Function> defFunc(misc::position const& pos
                                   , std::string const& name
                                   , std::vector<std::string> const& param_names
                                   , bool hint_void_return);
        util::sptr<Expression const> queryCall(misc::position const& pos
                                             , std::string const& name
                                             , std::vector<util::sptr<Expression const>> args)
                                            const;
        util::sref<Function> queryFunc(misc::position const& pos
                                     , std::string const& name
                                     , int param_count) const;
        std::map<std::string, inst::Variable const> bindExternalVars(
                                              misc::position const& pos
                                            , util::sref<inst::Scope const> ext_scope) const;
    private:
        std::map<std::string, std::list<misc::position>> _external_var_refs;
        std::map<std::string, misc::position> _VarDefs;

        Overloads _overloads;
        std::list<Function> _func_entities;
    private:
        static Function _fake_prototype;
    };

}

#endif /* __STEKIN_PROTO_SYMBOL_TABLE_H__ */
