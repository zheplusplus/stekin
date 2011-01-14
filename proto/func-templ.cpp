#include <algorithm>
#include <list>

#include "func-templ.h"
#include "func-node-factories.h"
#include "expr-nodes.h"
#include "stmt-nodes.h"
#include "err-report.h"
#include "../util/map-compare.h"

using namespace proto;

func_templ::func_templ(misc::pos_type const& ps
                     , std::string const& name
                     , std::vector<std::string> const& params)
    : pos(ps)
    , name(name)
    , param_names(params)
    , _factory(&_symbols)
    , body_scope(&_factory, &_block, &_symbols)
{
    _fill_param_names();
}

func_templ::func_templ(misc::pos_type const& ps
                     , std::string const& name
                     , std::vector<std::string> const& params
                     , symbol_table const* container_symbols)
    : pos(ps)
    , name(name)
    , param_names(params)
    , _symbols(container_symbols)
    , _factory(&_symbols)
    , body_scope(&_factory, &_block, &_symbols)
{
    _fill_param_names();
}

func_templ::func_templ(func_templ&& rhs)
    : pos(rhs.pos)
    , name(rhs.name)
    , param_names(rhs.param_names)
    , _symbols(std::move(rhs._symbols))
    , _factory(&_symbols)
    , body_scope(&_factory, &_block, &_symbols)
{}

void func_templ::_fill_param_names()
{
    std::for_each(param_names.begin()
                , param_names.end()
                , [&](std::string const& param_name)
                  {
                      _symbols.def_var(pos, param_name);
                  });
}

util::sref<inst::function> func_templ::inst(misc::pos_type const& pos
                                          , inst::scope const* ext_scope
                                          , std::vector<inst::type const*> const& arg_types)
{
    std::map<std::string, inst::variable const> ext_vars = _bind_external_vars(pos, ext_scope);
    auto find_result = _instance_cache.find(instance_info(ext_vars, arg_types));
    if (_instance_cache.end() != find_result) {
        util::sref<inst::function> instance = find_result->second;
        if (!instance->is_return_type_resolved()) {
            func_ret_type_unresolvable(name, arg_types.size());
        }
        return instance;
    }

    util::sref<inst::function> instance = inst::function
                ::create_instance(arg_types, ext_vars, RETURN_NO_VOID != body_scope.as_stmt()->termination());

    std::list<inst::arg_name_type_pair> args;
    for (unsigned i = 0; i < arg_types.size(); ++i) {
        args.push_back(inst::arg_name_type_pair(param_names[i], arg_types[i]));
    }

    _instance_cache.insert(std::make_pair(instance_info(ext_vars, arg_types), instance));
    inst::symbol_table sub_sym(ext_scope->level(), args, ext_vars);
    inst::scope sub_scope(instance, util::sref<inst::symbol_table>(&sub_sym));
    sub_scope.add_stmt(std::move(util::mkptr(body_scope.as_stmt()->inst(&sub_scope))));
    return instance;
}

std::map<std::string, inst::variable const>
    func_templ::_bind_external_vars(misc::pos_type const& pos, inst::scope const* ext_scope) const
{
    return _symbols.bind_external_var_refs(pos, ext_scope);
}

bool func_templ::instance_info::operator<(func_templ::instance_info const& rhs) const
{
    if (arg_types != rhs.arg_types) {
        return arg_types < rhs.arg_types;
    }
    return util::map_less(ext_vars, rhs.ext_vars);
}