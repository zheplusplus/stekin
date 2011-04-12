#include <algorithm>

#include "symbol-table.h"
#include "function.h"
#include "node-base.h"
#include "expr-nodes.h"
#include "../instance/node-base.h"
#include "../instance/scope.h"
#include "../instance/variable.h"
#include "../report/errors.h"

using namespace proto;

util::sref<function> overloads::query_or_null_if_nonexist(int param_count) const
{
    auto func = _funcs.find(param_count);
    if (_funcs.end() == func) {
        return util::sref<function>(NULL);
    }
    return func->second;
}

void overloads::declare(util::sref<function> func)
{
    auto insert_result = _funcs.insert(std::make_pair(func->param_names.size(), func));
    if (!insert_result.second) {
        error::func_already_in_local(insert_result.first->second->pos
                                   , func->pos
                                   , func->name
                                   , func->param_names.size());
    }
}

int overloads::count() const
{
    return int(_funcs.size());
}

util::sref<function> overloads::first() const
{
    return _funcs.begin()->second;
}

util::sptr<expr_base const> symbol_table::ref_var(misc::pos_type const& pos, std::string const& name)
{
    util::sref<overloads> ov = _find_overloads(name);
    if (1 == ov->count()) {
        return std::move(util::mkptr(new func_reference(pos, ov->first())));
    }

    if (_var_defs.end() == _var_defs.find(name)) {
        _external_var_refs[name].push_back(pos);
    }
    return std::move(util::mkptr(new reference(pos, name)));
}

void symbol_table::def_var(misc::pos_type const& pos, std::string const& name)
{
    auto local_refs = _external_var_refs.find(name);
    if (_external_var_refs.end() != local_refs) {
        error::var_ref_before_def(pos, local_refs->second, name);
    }
    auto insert_result = _var_defs.insert(std::make_pair(name, pos));
    if (!insert_result.second) {
        error::var_already_in_local(insert_result.first->second, pos, name);
    }
}

util::sref<function> symbol_table::def_func(misc::pos_type const& pos
                                          , std::string const& name
                                          , std::vector<std::string> const& params
                                          , bool hint_void_return)
{
    util::sref<function> func_in_external = _query_func_in_external_or_null_if_nonexist(name, params.size());
    if (bool(func_in_external)) {
        error::func_shadow_external(func_in_external->pos, pos, name, params.size());
        return func_in_external;
    }

    _func_entities.push_back(std::move(function(pos, name, params, util::mkref(*this), hint_void_return)));
    _find_overloads(name)->declare(util::mkref(_func_entities.back()));
    return util::mkref(_func_entities.back());
}

util::sptr<expr_base const> symbol_table::query_call(misc::pos_type const& pos
                                                   , std::string const& name
                                                   , std::vector<util::sptr<expr_base const>> args) const
{
    util::sref<function> func = _query_func_or_null_if_nonexist(name, args.size());
    if (bool(func)) {
        return std::move(util::mkptr(new call(pos, func, std::move(args))));
    }
    auto local_ref = _var_defs.find(name);
    if (_var_defs.end() != local_ref) {
        return std::move(util::mkptr(new functor(pos, name, std::move(args))));
    }
    error::func_not_def(pos, name, args.size());
    return std::move(util::mkptr(new call(pos, util::mkref(_fake_prototype), std::move(args))));
}

util::sref<function> symbol_table::query_func(misc::pos_type const& pos
                                            , std::string const& name
                                            , int param_count) const
{
    util::sref<function> func = _query_func_or_null_if_nonexist(name, param_count);
    if (bool(func)) {
        return func;
    }
    error::func_not_def(pos, name, param_count);
    return util::mkref(_fake_prototype);
}

std::map<std::string, inst::variable const>
        symbol_table::bind_external_var_refs(misc::pos_type const& pos
                                           , util::sref<inst::scope const> ext_scope) const
{
    std::map<std::string, inst::variable const> result;
    std::for_each(_external_var_refs.begin()
                , _external_var_refs.end()
                , [&](std::pair<std::string, std::list<misc::pos_type>> const& ref)
                  {
                      result.insert(std::make_pair(ref.first, ext_scope->query_var(pos, ref.first)));
                  });
    return result;
}

util::sref<overloads> symbol_table::_find_overloads(std::string const& name)
{
    auto ov = std::find_if(_overloads.begin()
                         , _overloads.end()
                         , [&](overloads const& o)
                           {
                               return o.name == name;
                           });
    if (ov == _overloads.end()) {
        _overloads.push_back(overloads(name));
        return util::mkref(_overloads.back());
    }
    return util::mkref(*ov);
}

util::sref<function> symbol_table::_query_func_or_null_if_nonexist(std::string const& name
                                                                 , int param_count) const
{
    auto ov = std::find_if(_overloads.begin()
                         , _overloads.end()
                         , [&](overloads const& o)
                           {
                               return o.name == name;
                           });
    if (ov != _overloads.end()) {
        util::sref<function> func = ov->query_or_null_if_nonexist(param_count);
        if (bool(func)) {
            return func;
        }
    }
    return _query_func_in_external_or_null_if_nonexist(name, param_count);
}

util::sref<function> symbol_table::_query_func_in_external_or_null_if_nonexist(std::string const& name
                                                                             , int param_count) const
{
    return bool(_external_or_null_on_global)
              ? _external_or_null_on_global->_query_func_or_null_if_nonexist(name, param_count)
              : util::sref<function>(0);
}

static symbol_table fake_symbols;
function symbol_table::_fake_prototype(misc::pos_type(0)
                                     , ""
                                     , std::vector<std::string>()
                                     , util::mkref(fake_symbols)
                                     , false);
