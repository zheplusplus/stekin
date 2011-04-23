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

static std::vector<util::sref<Function>> appendFuncs(std::vector<util::sref<Function>> a
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

util::sref<Function> Overloads::Overload::queryOrNulIfNonexist(int param_count) const
{
    auto func = _funcs.find(param_count);
    if (_funcs.end() == func) {
        return util::sref<Function>(NULL);
    }
    return func->second;
}

void Overloads::Overload::declare(util::sref<Function> func)
{
    _funcs.insert(std::make_pair(func->param_names.size(), func));
}

std::vector<util::sref<Function>> Overloads::Overload::all() const
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

std::vector<util::sref<Function>> Overloads::allFuncsOfName(std::string const& name) const
{
    std::vector<util::sref<Function>> external_funcs_of_name(
                                bool(_external_overloads_or_nul_on_global)
                              ? (_external_overloads_or_nul_on_global->allFuncsOfName(name))
                              : (std::vector<util::sref<Function>>()));
    util::sref<Overload const> o = _overloadByNameOrNulIfNonexist(name);
    if (bool(o)) {
        return appendFuncs(std::move(external_funcs_of_name), o->all());
    }
    return appendFuncs(std::move(external_funcs_of_name), std::vector<util::sref<Function>>());
}

util::sref<Function> Overloads::queryOrNulIfNonexist(std::string const& name, int param_count) const
{
    util::sref<Overload const> o = _overloadByNameOrNulIfNonexist(name);
    if (bool(o)) {
        util::sref<Function> f = o->queryOrNulIfNonexist(param_count);
        if (bool(f)) {
            return f;
        }
    }
    if (bool(_external_overloads_or_nul_on_global)) {
        return _external_overloads_or_nul_on_global->queryOrNulIfNonexist(name, param_count);
    }
    return util::sref<Function>(NULL);
}

void Overloads::declare(util::sref<Function> func)
{
    _overloadByName(func->name)->declare(func);
}

util::sref<Overloads::Overload> Overloads::_overloadByName(std::string const& name)
{
    auto i = std::find_if(_overloads.begin()
                        , _overloads.end()
                        , [&](Overload const& o)
                          {
                              return name == o.name;
                          });
    if (i != _overloads.end()) {
        return util::mkref(*i);
    }
    _overloads.push_back(Overload(name));
    return util::mkref(_overloads.back());
}

util::sref<Overloads::Overload const>
        Overloads::_overloadByNameOrNulIfNonexist(std::string const& name) const
{
    auto i = std::find_if(_overloads.begin()
                        , _overloads.end()
                        , [&](Overload const& o)
                          {
                              return name == o.name;
                          });
    if (i != _overloads.end()) {
        return util::mkref(*i);
    }
    return util::sref<Overload const>(NULL);
}

util::sptr<Expression const> SymbolTable::refVar(misc::position const& pos, std::string const& name)
{
    std::vector<util::sref<Function>> all_funcs = _overloads.allFuncsOfName(name);
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
        error::varRefBeforeDef(pos, local_refs->second, name);
    }
    auto insert_result = _VarDefs.insert(std::make_pair(name, pos));
    if (!insert_result.second) {
        error::varAlreadyInLocal(insert_result.first->second, pos, name);
    }
}

util::sref<Function> SymbolTable::defFunc(misc::position const& pos
                                        , std::string const& name
                                        , std::vector<std::string> const& params
                                        , bool hint_void_return)
{
    util::sref<Function> func = _overloads.queryOrNulIfNonexist(name, params.size());
    if (bool(func)) {
        error::funcAlreadyDef(func->pos, pos, name, params.size());
        return func;
    }

    _func_entities.push_back(std::move(Function(pos
                                              , name
                                              , params
                                              , util::mkref(*this)
                                              , hint_void_return)));
    _overloads.declare(util::mkref(_func_entities.back()));
    return util::mkref(_func_entities.back());
}

util::sptr<Expression const> SymbolTable::queryCall(misc::position const& pos
                                                  , std::string const& name
                                                  , std::vector<util::sptr<Expression const>> args)
                                                const
{
    util::sref<Function> func = _overloads.queryOrNulIfNonexist(name, args.size());
    if (bool(func)) {
        return std::move(util::mkptr(new Call(pos, func, std::move(args))));
    }
    auto local_ref = _VarDefs.find(name);
    if (_VarDefs.end() != local_ref) {
        return std::move(util::mkptr(new Functor(pos, name, std::move(args))));
    }
    error::funcNotDef(pos, name, args.size());
    return std::move(util::mkptr(new Call(pos, util::mkref(_fake_prototype), std::move(args))));
}

util::sref<Function> SymbolTable::queryFunc(misc::position const& pos
                                          , std::string const& name
                                          , int param_count) const
{
    util::sref<Function> func = _overloads.queryOrNulIfNonexist(name, param_count);
    if (bool(func)) {
        return func;
    }
    error::funcNotDef(pos, name, param_count);
    return util::mkref(_fake_prototype);
}

std::map<std::string, inst::Variable const> SymbolTable::bindExternalVars(
                                                      misc::position const& pos
                                                    , util::sref<inst::Scope const> ext_scope) const
{
    std::map<std::string, inst::Variable const> result;
    std::for_each(_external_var_refs.begin()
                , _external_var_refs.end()
                , [&](std::pair<std::string, std::list<misc::position>> const& ref)
                  {
                      result.insert(std::make_pair(ref.first, ext_scope->queryVar(pos, ref.first)));
                  });
    return result;
}

static SymbolTable fake_symbols;
Function SymbolTable::_fake_prototype(misc::position(0)
                                    , ""
                                    , std::vector<std::string>()
                                    , util::mkref(fake_symbols)
                                    , false);
