#ifndef __STEKIN_FLOWCHECK_SYMBOL_TABLE_H__
#define __STEKIN_FLOWCHECK_SYMBOL_TABLE_H__

#include <string>
#include <map>
#include <list>
#include <vector>

#include "fwd-decl.h"
#include "../proto/fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace flchk {

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
        explicit SymbolTable(util::sref<SymbolTable const> ext_symbols)
            : _overloads(util::mkref(ext_symbols->_overloads))
        {}

        SymbolTable()
            : _overloads(util::sref<Overloads const>(NULL))
        {}

        SymbolTable(SymbolTable&& rhs)
            : _external_var_refs(rhs._external_var_refs)
            , _var_defs(rhs._var_defs)
            , _overloads(rhs._overloads)
        {}

        void refVars(misc::position const& pos, std::vector<std::string> const& vars);
        void defVar(misc::position const& pos, std::string const& name);
        void defFunc(util::sref<Function> func);

        util::sptr<proto::Expression const> compileRef(misc::position const& pos
                                                     , std::string const& name
                                                     , util::sref<proto::Block> block);
        util::sptr<proto::Expression const> compileCall(
                        misc::position const& pos
                      , util::sref<proto::Block> block
                      , std::string const& name
                      , std::vector<util::sptr<Expression const>> const& args);
        util::sref<Function> queryFunc(misc::position const& pos
                                     , std::string const& name
                                     , int param_count);
        std::vector<std::string> freeVariables() const;
    private:
        void _markReference(misc::position const& pos, std::string const& name);
        std::vector<util::sptr<proto::Expression const>> _mkArgs(
                        std::vector<util::sptr<Expression const>> const& args
                      , util::sref<proto::Block> block);
        util::sref<proto::Function> _compileFunction(misc::position const& pos
                                                   , util::sref<Function> func
                                                   , util::sref<proto::Block> block);
        util::sptr<proto::Expression const> _compileAsFunctor(
                        misc::position const& pos
                      , std::string const& name
                      , util::sref<proto::Block> block
                      , std::vector<util::sptr<Expression const>> const& args);
    private:
        std::map<std::string, std::list<misc::position>> _external_var_refs;
        std::map<std::string, misc::position> _var_defs;

        Overloads _overloads;
    private:
        static Function _fake_function;
    };

}

#endif /* __STEKIN_FLOWCHECK_SYMBOL_TABLE_H__ */
