#ifndef __STEKIN_INSTANCE_FUNCTION_H__
#define __STEKIN_INSTANCE_FUNCTION_H__

#include <map>
#include <list>
#include <string>

#include "scope.h"
#include "stmt-nodes.h"
#include "type.h"
#include "inst-mediate.h"
#include "symbol-table.h"
#include "../misc/pos-type.h"

namespace inst {

    struct Function
        : public scope
    {
        variable def_var(misc::pos_type const& pos, util::sref<type const> vtype, std::string const& name);
        variable query_var(misc::pos_type const& pos, std::string const& name) const;

        void set_return_type(misc::pos_type const& pos, util::sref<type const> return_type);
        virtual util::sref<type const> get_return_type() const;
        virtual bool is_return_type_resolved() const;

        void add_path(util::sref<mediate_base> path);
        void inst_next_path();
        bool has_more_path() const;

        int level() const;
    public:
        static util::sref<Function> create_instance(int ext_level
                                                  , std::list<arg_name_type_pair> const& args
                                                  , std::map<std::string, variable const> const& extvars
                                                  , bool has_void_returns);

        static void write_decls();
        static void write_impls();
    protected:
        Function(int ext_level
               , std::list<arg_name_type_pair> const& args
               , std::map<std::string, variable const> const& ext_vars)
            : _symbols(ext_level, args, ext_vars)
        {}
    protected:
        Function(Function const&) = delete;
    private:
        std::list<util::sref<mediate_base>> _candidate_paths;
        symbol_table _symbols;
    };

}

#endif /* __STEKIN_INSTANCE_FUNCTION_H__ */
