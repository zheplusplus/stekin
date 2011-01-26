#include <list>
#include <set>
#include <algorithm>

#include "function.h"
#include "err-report.h"
#include "../util/map-compare.h"
#include "../util/pointer.h"

using namespace inst;

variable function::def_var(misc::pos_type const& pos, type const* vtype, std::string const& name)
{
    return _symbols.def_var(pos, vtype, name);
}

variable function::query_var(misc::pos_type const& pos, std::string const& name) const
{
    return _symbols.query_var(pos, name);
}

type const* function::get_return_type() const
{
    return type::BIT_VOID;
}

void function::set_return_type(misc::pos_type const& pos, type const* return_type)
{
    if (type::BIT_VOID != return_type) {
        conflict_return_type(pos, type::BIT_VOID->name, return_type->name);
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

        type const* get_return_type() const
        {
            return _return_type;
        }

        void set_return_type(misc::pos_type const& pos, type const* return_type)
        {
            if (type::BAD_TYPE == _return_type) {
                _return_type = return_type;
                return;
            }
            if (_return_type != return_type) {
                conflict_return_type(pos, _return_type->name, return_type->name);
            }
        }

        bool is_return_type_resolved() const
        {
            return type::BAD_TYPE != _return_type;
        }

        type const* _return_type;
    };

    struct func_inst_recs
        : protected std::list<std::unique_ptr<function const>>
    {
        void add(util::sptr<function const> func)
        {
            push_back(std::move(func));
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
    if (_candidate_paths.empty()) {
        return;
    }
    util::sref<mediate_base> next_path = _candidate_paths.front();
    _candidate_paths.pop_front();
    next_path->mediate_inst(util::mkref(*this));
}

int function::level() const
{
    return _symbols.level;
}
