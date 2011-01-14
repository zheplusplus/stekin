#include "symbol-table.h"
#include "func-templ.h"
#include "err-report.h"

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
        var_ref_before_def(pos, local_refs->second, name);
    }
    auto insert_result = _var_defs.insert(std::make_pair(name, pos));
    if (!insert_result.second) {
        var_already_in_local(insert_result.first->second, pos, name);
    }
}

func_templ* symbol_table::def_func(misc::pos_type const& pos
                                 , std::string const& name
                                 , std::vector<std::string> const& params)
{
    _func_entities.push_back(_make_func(pos, name, params));
    auto insert_result = _funcs.insert(std::make_pair(func_signature(name, params.size())
                                                    , &_func_entities.back()));
    if (!insert_result.second) {
        func_already_in_local(insert_result.first->second->pos, pos, name, params.size());
        _func_entities.pop_back();
    }
    func_templ* func_in_external
          = _query_func_in_external_or_null_if_nonexist(name, params.size());
    if (NULL != func_in_external) {
        func_shadow_external(func_in_external->pos, pos, name, params.size());
    }
    return insert_result.first->second;
}

func_templ* symbol_table::query_func(misc::pos_type const& pos, std::string const& name, int param_count) const
{
    func_templ* func = _query_func_or_null_if_nonexist(name, param_count);
    if (NULL != func) {
        return func;
    }
    func_not_def(pos, name, param_count);
    return &_fake_prototype;
}

std::map<std::string, inst::variable const>
    symbol_table::bind_external_var_refs(misc::pos_type const& pos, inst::scope const* ext_scope) const
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

func_templ symbol_table::_make_func(misc::pos_type const& pos
                                  , std::string const& name
                                  , std::vector<std::string> const& params)
{
    return func_templ(pos, name, params, this);
}

func_templ*
    symbol_table::_query_func_or_null_if_nonexist(std::string const& name, int param_count) const
{
    auto find_result = _funcs.find(func_signature(name, param_count));
    if (_funcs.end() != find_result) {
        return find_result->second;
    }
    return _query_func_in_external_or_null_if_nonexist(name, param_count);
}

func_templ*
    symbol_table::_query_func_in_external_or_null_if_nonexist(std::string const& name, int param_count) const
{
    return NULL == _external_or_null_on_global
                 ? NULL
                 : _external_or_null_on_global->_query_func_or_null_if_nonexist(name, param_count);
}

func_templ symbol_table::_fake_prototype(misc::pos_type(0), "", std::vector<std::string>());
