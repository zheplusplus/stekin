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

    struct overloads {
        explicit overloads(util::sref<overloads const> external_overloads)
            : _external_overloads_or_null_on_global(external_overloads)
        {}
    private:
        struct overload {
            explicit overload(std::string const& n)
                : name(n)
            {}

            std::string const name;
        public:
            util::sref<Function> query_or_null_if_nonexist(int param_count) const;
            void declare(util::sref<Function> func);
            std::vector<util::sref<Function>> all() const;
        private:
            std::map<int, util::sref<Function> const> _funcs;
        };
    public:
        std::vector<util::sref<Function>> all_funcs_of_name(std::string const& name) const;
        util::sref<Function> query_or_null_if_nonexist(std::string const& name, int param_count) const;
        void declare(util::sref<Function> func);
    private:
        util::sref<overload> _overload_by_name(std::string const& name);
        util::sref<overload const> _overload_by_name_or_null_if_nonexist(std::string const& name) const;
    private:
        util::sref<overloads const> const _external_overloads_or_null_on_global;
        std::list<overload> _overloads;
    };

    struct symbol_table {
        int const level;

        explicit symbol_table(util::sref<symbol_table const> ext_symbols)
            : level(ext_symbols->level + 1)
            , _overloads(util::mkref(ext_symbols->_overloads))
        {}

        symbol_table()
            : level(0)
            , _overloads(util::sref<overloads const>(NULL))
        {}

        symbol_table(symbol_table&& rhs)
            : level(rhs.level)
            , _external_var_refs(rhs._external_var_refs)
            , _VarDefs(rhs._VarDefs)
            , _overloads(rhs._overloads)
            , _func_entities(std::move(rhs._func_entities))
        {}

        util::sptr<Expression const> ref_var(misc::position const& pos, std::string const& name);
        void defVar(misc::position const& pos, std::string const& name);
        util::sref<Function> defFunc(misc::position const& pos
                                    , std::string const& name
                                    , std::vector<std::string> const& param_names
                                    , bool hint_void_return);
        util::sptr<Expression const> query_call(misc::position const& pos
                                             , std::string const& name
                                             , std::vector<util::sptr<Expression const>> args) const;
        util::sref<Function> query_func(misc::position const& pos
                                      , std::string const& name
                                      , int param_count) const;
        std::map<std::string, inst::variable const>
            bind_external_var_refs(misc::position const& pos, util::sref<inst::scope const> ext_scope) const;
    private:
        std::map<std::string, std::list<misc::position>> _external_var_refs;
        std::map<std::string, misc::position> _VarDefs;

        overloads _overloads;
        std::list<Function> _func_entities;
    private:
        static Function _fake_prototype;
    };

}

#endif /* __STEKIN_PROTO_SYMBOL_TABLE_H__ */
