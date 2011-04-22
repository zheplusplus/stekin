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

static std::vector<util::sref<Function>> append_funcs(std::vector<util::sref<Function>> a
                                                    , std::vector<util::sref<Function>> b)
{
    std::for_each(b.begin()
                , b.end()
                , [&](util::sref<Function> f)
                  {
                      a.push_back(f);
                  });
    return std::move(a);
}

util::sref<Function> overloads::overload::query_or_null_if_nonexist(int param_count) const
{
    auto func = _funcs.find(param_count);
    if (_funcs.end() == func) {
        return util::sref<Function>(NULL);
    }
    return func->second;
}

void overloads::overload::declare(util::sref<Function> func)
{
    _funcs.insert(std::make_pair(func->param_names.size(), func));
}

std::vector<util::sref<Function>> overloads::overload::all() const
{
    std::vector<util::sref<Function>> result_funcs;
    std::for_each(_funcs.begin()
                , _funcs.end()
                , [&](std::pair<int, util::sref<Function>> const& func)
                  {
                      result_funcs.push_back(func.second);
                  });
    return result_funcs;
}

std::vector<util::sref<Function>> overloads::all_funcs_of_name(std::string const& name) const
{
    std::vector<util::sref<Function>> external_funcs_of_name(
                                bool(_external_overloads_or_null_on_global)
                              ? (_external_overloads_or_null_on_global->all_funcs_of_name(name))
                              : (std::vector<util::sref<Function>>()));
    util::sref<overload const> o = _overload_by_name_or_null_if_nonexist(name);
    if (bool(o)) {
        return append_funcs(std::move(external_funcs_of_name), o->all());
    }
    return append_funcs(std::move(external_funcs_of_name), std::vector<util::sref<Function>>());
}

util::sref<Function> overloads::query_or_null_if_nonexist(std::string const& name, int param_count) const
{
    util::sref<overload const> o = _overload_by_name_or_null_if_nonexist(name);
    if (bool(o)) {
        util::sref<Function> f = o->query_or_null_if_nonexist(param_count);
        if (bool(f)) {
            return f;
        }
    }
    if (bool(_external_overloads_or_null_on_global)) {
        return _external_overloads_or_null_on_global->query_or_null_if_nonexist(name, param_count);
    }
    return util::sref<Function>(NULL);
}

void overloads::declare(util::sref<Function> func)
{
    _overload_by_name(func->name)->declare(func);
}

util::sref<overloads::overload> overloads::_overload_by_name(std::string const& name)
{
    auto i = std::find_if(_overloads.begin()
                        , _overloads.end()
                        , [&](overload const& o)
                          {
                              return name == o.name;
                          });
    if (i != _overloads.end()) {
        return util::mkref(*i);
    }
    _overloads.push_back(overload(name));
    return util::mkref(_overloads.back());
}

util::sref<overloads::overload const>
        overloads::_overload_by_name_or_null_if_nonexist(std::string const& name) const
{
    auto i = std::find_if(_overloads.begin()
                        , _overloads.end()
                        , [&](overload const& o)
                          {
                              return name == o.name;
                          });
    if (i != _overloads.end()) {
        return util::mkref(*i);
    }
    return util::sref<overload const>(NULL);
}

util::sptr<Expression const> SymbolTable::ref_var(misc::position const& pos, std::string const& name)
{
    std::vector<util::sref<Function>> all_funcs = _overloads.all_funcs_of_name(name);
    if (!all_funcs.empty()) {
        if (all_funcs.size() > 1) {
            error::funcReferenceAmbiguous(pos, name);
        }
        return std::move(util::mkptr(new FuncReference(pos, all_funcs[0])));
    }

    if (_VarDefs.end() == _VarDefs.find(name)) {
        _external_var_refs[name].push_back(pos);
    }
    return std::move(util::mkptr(new Reference(pos, name)));
}

void SymbolTable::defVar(misc::position const& pos, std::string const& name)
{
    auto local_refs = _external_var_refs.find(name);
    if (_external_var_refs.end() != local_refs) {
        error::var_ref_before_def(pos, local_refs->second, name);
    }
    auto insert_result = _VarDefs.insert(std::make_pair(name, pos));
    if (!insert_result.second) {
        error::var_already_in_local(insert_result.first->second, pos, name);
    }
}

util::sref<Function> SymbolTable::defFunc(misc::position const& pos
                                          , std::string const& name
                                          , std::vector<std::string> const& params
                                          , bool hint_void_return)
{
    util::sref<Function> func = _overloads.query_or_null_if_nonexist(name, params.size());
    if (bool(func)) {
        error::func_already_def(func->pos, pos, name, params.size());
        return func;
    }

    _func_entities.push_back(std::move(Function(pos, name, params, util::mkref(*this), hint_void_return)));
    _overloads.declare(util::mkref(_func_entities.back()));
    return util::mkref(_func_entities.back());
}

util::sptr<Expression const> SymbolTable::query_call(misc::position const& pos
                                                   , std::string const& name
                                                   , std::vector<util::sptr<Expression const>> args) const
{
    util::sref<Function> func = _overloads.query_or_null_if_nonexist(name, args.size());
    if (bool(func)) {
        return std::move(util::mkptr(new call(pos, func, std::move(args))));
    }
    auto local_ref = _VarDefs.find(name);
    if (_VarDefs.end() != local_ref) {
        return std::move(util::mkptr(new functor(pos, name, std::move(args))));
    }
    error::func_not_def(pos, name, args.size());
    return std::move(util::mkptr(new call(pos, util::mkref(_fake_prototype), std::move(args))));
}

util::sref<Function> SymbolTable::query_func(misc::position const& pos
                                            , std::string const& name
                                            , int param_count) const
{
    util::sref<Function> func = _overloads.query_or_null_if_nonexist(name, param_count);
    if (bool(func)) {
        return func;
    }
    error::func_not_def(pos, name, param_count);
    return util::mkref(_fake_prototype);
}

std::map<std::string, inst::variable const>
        SymbolTable::bind_external_var_refs(misc::position const& pos
                                           , util::sref<inst::Scope const> ext_scope) const
{
    std::map<std::string, inst::variable const> result;
    std::for_each(_external_var_refs.begin()
                , _external_var_refs.end()
                , [&](std::pair<std::string, std::list<misc::position>> const& ref)
                  {
                      result.insert(std::make_pair(ref.first, ext_scope->query_var(pos, ref.first)));
                  });
    return result;
}

static SymbolTable fake_symbols;
Function SymbolTable::_fake_prototype(misc::position(0)
                                     , ""
                                     , std::vector<std::string>()
                                     , util::mkref(fake_symbols)
                                     , false);
