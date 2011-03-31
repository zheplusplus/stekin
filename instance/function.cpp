#include <list>
#include <set>
#include <algorithm>

#include "node-base.h"
#include "function.h"
#include "../report/errors.h"
#include "../output/func-writer.h"
#include "../util/map-compare.h"
#include "../util/pointer.h"

using namespace inst;

variable function::def_var(misc::pos_type const& pos, util::sref<type const> vtype, std::string const& name)
{
    return _symbols.def_var(pos, vtype, name);
}

variable function::query_var(misc::pos_type const& pos, std::string const& name) const
{
    return _symbols.query_var(pos, name);
}

util::sref<type const> function::get_return_type() const
{
    return type::BIT_VOID;
}

void function::set_return_type(misc::pos_type const& pos, util::sref<type const> return_type)
{
    if (type::BIT_VOID != return_type) {
        error::conflict_return_type(pos, type::BIT_VOID->name(), return_type->name());
    }
}

bool function::is_return_type_resolved() const
{
    return true;
}

namespace {

    struct function_unresolved
        : public function
    {
        function_unresolved(int ext_level
                          , std::list<arg_name_type_pair> const& args
                          , std::map<std::string, variable const> const& extvars)
            : function(ext_level, args, extvars)
            , _return_type(type::BAD_TYPE)
        {}

        util::sref<type const> get_return_type() const
        {
            return _return_type;
        }

        void set_return_type(misc::pos_type const& pos, util::sref<type const> return_type)
        {
            if (type::BAD_TYPE == _return_type) {
                _return_type = return_type;
                return;
            }
            if (_return_type != return_type) {
                error::conflict_return_type(pos, _return_type->name(), return_type->name());
            }
        }

        bool is_return_type_resolved() const
        {
            return type::BAD_TYPE != _return_type;
        }

        util::sref<type const> _return_type;
    };

    struct func_inst_recs
        : protected std::list<util::sptr<function const>>
    {
        typedef std::list<util::sptr<function const>> base_type;
        typedef base_type::const_iterator iterator;

        void add(util::sptr<function const> func)
        {
            push_back(std::move(func));
        }

        iterator begin() const
        {
            return base_type::begin();
        }

        iterator end() const
        {
            return base_type::end();
        }
    private:
        func_inst_recs() {}
    public:
        static func_inst_recs instance;
    };

    func_inst_recs func_inst_recs::instance;

}

util::sref<function> function::create_instance(int ext_level
                                             , std::list<arg_name_type_pair> const& arg_types
                                             , std::map<std::string, variable const> const& extvars
                                             , bool has_void_returns)
{
    util::sptr<function> func(has_void_returns ? new function(ext_level, arg_types, extvars)
                                               : new function_unresolved(ext_level, arg_types, extvars));
    util::sref<function> fref = *func;
    func_inst_recs::instance.add(std::move(func));
    return fref;
}

void function::add_path(util::sref<mediate_base> path)
{
    _candidate_paths.push_back(path);
}

void function::inst_next_path()
{
    if (!has_more_path()) {
        return;
    }
    util::sref<mediate_base> next_path = _candidate_paths.front();
    _candidate_paths.pop_front();
    next_path->mediate_inst(util::mkref(*this));
}

bool function::has_more_path() const
{
    return !_candidate_paths.empty();
}

int function::level() const
{
    return _symbols.level;
}

static std::list<output::stack_var_record> args_to_var_recs(std::list<inst::variable> const& args)
{
    std::list<output::stack_var_record> recs;
    std::for_each(args.begin()
                , args.end()
                , [&](inst::variable const& var)
                  {
                      recs.push_back(output::stack_var_record(var.vtype->exported_name()
                                                            , var.stack_offset
                                                            , var.level));
                  });
    return recs;
}

void function::write_decls()
{
    std::for_each(func_inst_recs::instance.begin()
                , func_inst_recs::instance.end()
                , [&](util::sptr<function const> const& func)
                  {
                      output::write_func_decl(func->get_return_type()->exported_name()
                                            , func.id()
                                            , args_to_var_recs(func->_symbols.get_args())
                                            , func->_symbols.level
                                            , func->_symbols.stack_size());
                  });
}

void function::write_impls()
{
    std::for_each(func_inst_recs::instance.begin()
                , func_inst_recs::instance.end()
                , [&](util::sptr<function const> const& func)
                  {
                      output::write_func_perform_impl(func->get_return_type()->exported_name(), func.id());
                      func->_block.write();
                  });
}
