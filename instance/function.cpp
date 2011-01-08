#include <list>
#include <set>
#include <algorithm>

#include "function.h"
#include "err-report.h"
#include "../util/map-compare.h"
#include "../util/pointer.h"

using namespace inst;

bool function::operator<(function const& rhs) const
{
    if (arg_types != rhs.arg_types) {
        return arg_types < rhs.arg_types;
    }

    return util::map_less(external_vars, rhs.external_vars);
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
        function_unresolved(std::vector<type const*> const& arg_types
                          , std::map<std::string, variable const> const& extvars)
            : function(arg_types, extvars)
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
        void add(util::sptr<function const>&& func)
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

util::sref<function> function::create_instance(std::vector<type const*> const& arg_types
                                             , std::map<std::string, variable const> const& extvars
                                             , bool has_void_returns)
{
    util::sptr<function> func(
            has_void_returns ? new function(arg_types, extvars) : new function_unresolved(arg_types, extvars)
                             );
    util::sref<function> fref = *func;
    func_inst_recs::instance.add(std::move(func));
    return fref;
}
