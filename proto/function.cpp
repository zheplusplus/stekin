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

Function::Function(misc::position const& ps
                 , std::string const& name
                 , std::vector<std::string> const& params
                 , util::sref<symbol_table const> ext_symbols
                 , bool func_hint_void_return)
    : general_scope(ext_symbols)
    , pos(ps)
    , name(name)
    , param_names(params)
    , hint_void_return(func_hint_void_return)
{
    _fill_param_names();
}

void Function::_fill_param_names()
{
    std::for_each(param_names.begin()
                , param_names.end()
                , [&](std::string const& param_name)
                  {
                      _symbols.defVar(pos, param_name);
                  });
}

util::sref<inst::Function> Function::inst(misc::position const& pos
                                        , util::sref<inst::scope> ext_scope
                                        , std::vector<util::sref<inst::type const>> const& arg_types)
{
    return inst(ext_scope->level(), bind_external_vars(pos, ext_scope), arg_types);
}

util::sref<inst::Function> Function::inst(int level
                                        , std::map<std::string, inst::variable const> const& ext_vars
                                        , std::vector<util::sref<inst::type const>> const& arg_types)
{
    auto find_result = _instance_cache.find(instance_info(ext_vars, arg_types));
    if (_instance_cache.end() != find_result) {
        util::sref<inst::Function> instance = find_result->second;
        while (!instance->is_return_type_resolved() && instance->has_more_path()) {
            instance->inst_next_path();
        }
        if (!instance->is_return_type_resolved()) {
            error::returnTypeUnresolvable(name, arg_types.size());
        }
        return instance;
    }

    std::list<inst::arg_name_type_pair> args;
    for (unsigned i = 0; i < arg_types.size(); ++i) {
        args.push_back(inst::arg_name_type_pair(param_names[i], arg_types[i]));
    }
    util::sref<inst::Function> instance = inst::Function::create_instance(level
                                                                        , args
                                                                        , ext_vars
                                                                        , hint_void_return);
    _instance_cache.insert(std::make_pair(instance_info(ext_vars, arg_types), instance));

    BlockMediate body_mediate(_block.get_stmts(), instance);
    instance->inst_next_path();
    instance->addStmt(std::move(body_mediate.inst(instance)));
    return instance;
}

std::map<std::string, inst::variable const>
    Function::bind_external_vars(misc::position const& pos, util::sref<inst::scope const> ext_scope) const
{
    return _symbols.bind_external_var_refs(pos, ext_scope);
}

bool Function::instance_info::operator<(Function::instance_info const& rhs) const
{
    if (arg_types != rhs.arg_types) {
        return arg_types < rhs.arg_types;
    }
    return util::map_less(ext_vars, rhs.ext_vars);
}
