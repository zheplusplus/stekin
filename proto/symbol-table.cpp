#include "symbol-table.h"
#include "function.h"
#include "../report/errors.h"

using namespace proto;

void symbol_table::ref_var(misc::pos_type const& pos, std::string const& name)
{
    if (_var_defs.end() == _var_defs.find(name)) {
        _external_var_refs[name].push_back(pos);
    }
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
                                          , std::vector<std::string> const& params)
{
    _func_entities.push_back(std::move(function(pos, name, params, util::mkref(*this))));
    auto insert_result = _funcs.insert(std::make_pair(func_signature(name, params.size())
                                                    , util::mkref(_func_entities.back())));
    if (!insert_result.second) {
        error::func_already_in_local(insert_result.first->second->pos, pos, name, params.size());
    }
    util::sref<function> func_in_external = _query_func_in_external_or_null_if_nonexist(name, params.size());
    if (bool(func_in_external)) {
        error::func_shadow_external(func_in_external->pos, pos, name, params.size());
    }
    return insert_result.first->second;
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

util::sref<function>
    symbol_table::_query_func_or_null_if_nonexist(std::string const& name, int param_count) const
{
    auto find_result = _funcs.find(func_signature(name, param_count));
    if (_funcs.end() != find_result) {
        return find_result->second;
    }
    return _query_func_in_external_or_null_if_nonexist(name, param_count);
}

util::sref<function>
    symbol_table::_query_func_in_external_or_null_if_nonexist(std::string const& name, int param_count) const
{
    return bool(_external_or_null_on_global)
                 ? _external_or_null_on_global->_query_func_or_null_if_nonexist(name, param_count)
                 : util::sref<function>(0);
}

function symbol_table::_fake_prototype(misc::pos_type(0), "", std::vector<std::string>());
