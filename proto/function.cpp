#include <algorithm>
#include <list>

#include "function.h"
#include "expr-nodes.h"
#include "stmt-nodes.h"
#include "inst-mediates.h"
#include "../instance/stmt-nodes.h"
#include "../instance/function.h"
#include "../util/map-compare.h"
#include "../report/errors.h"

using namespace proto;

function::function(misc::pos_type const& ps
                 , std::string const& name
                 , std::vector<std::string> const& params
                 , util::sref<symbol_table const> ext_symbols)
    : general_scope(ext_symbols)
    , pos(ps)
    , name(name)
    , param_names(params)
{
    _fill_param_names();
}

void function::_fill_param_names()
{
    std::for_each(param_names.begin()
                , param_names.end()
                , [&](std::string const& param_name)
                  {
                      _symbols.def_var(pos, param_name);
                  });
}

util::sref<inst::function> function::inst(misc::pos_type const& pos
                                        , util::sref<inst::scope> ext_scope
                                        , std::vector<inst::type const*> const& arg_types)
{
    std::map<std::string, inst::variable const> ext_vars = _bind_external_vars(pos, ext_scope);
    auto find_result = _instance_cache.find(instance_info(ext_vars, arg_types));
    if (_instance_cache.end() != find_result) {
        util::sref<inst::function> instance = find_result->second;
        while (!instance->is_return_type_resolved() && instance->has_more_path()) {
            instance->inst_next_path();
        }
        if (!instance->is_return_type_resolved()) {
            error::func_ret_type_unresolvable(name, arg_types.size());
        }
        return instance;
    }

    std::list<inst::arg_name_type_pair> args;
    for (unsigned i = 0; i < arg_types.size(); ++i) {
        args.push_back(inst::arg_name_type_pair(param_names[i], arg_types[i]));
    }
    util::sref<inst::function> instance
                = inst::function::create_instance(ext_scope->level()
                                                , args
                                                , ext_vars
                                                , RETURN_VOID == _status
                                               || PARTIAL_RETURN_VOID == _status
                                               || NO_EXPLICIT_TERMINATION == _status);
    _instance_cache.insert(std::make_pair(instance_info(ext_vars, arg_types), instance));

    block_mediate body_mediate(_block.get_stmts(), instance);
    instance->inst_next_path();
    instance->add_stmt(std::move(body_mediate.inst(instance)));
    return instance;
}

std::map<std::string, inst::variable const>
    function::_bind_external_vars(misc::pos_type const& pos, util::sref<inst::scope const> ext_scope) const
{
    return _symbols.bind_external_var_refs(pos, ext_scope);
}

bool function::instance_info::operator<(function::instance_info const& rhs) const
{
    if (arg_types != rhs.arg_types) {
        return arg_types < rhs.arg_types;
    }
    return util::map_less(ext_vars, rhs.ext_vars);
}
