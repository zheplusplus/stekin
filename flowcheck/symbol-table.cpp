#include <algorithm>

#include "symbol-table.h"
#include "function.h"
#include "expr-nodes.h"
#include "global-filter.h"
#include "../proto/variable.h"
#include "../proto/expr-nodes.h"
#include "../proto/func-inst-draft.h"
#include "../report/errors.h"

using namespace flchk;

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
                                _external_overloads_or_nul_on_global.not_nul()
                              ? (_external_overloads_or_nul_on_global->allFuncsOfName(name))
                              : (std::vector<util::sref<Function>>()));
    util::sref<Overload const> o = _overloadByNameOrNulIfNonexist(name);
    if (o.not_nul()) {
        return appendFuncs(std::move(external_funcs_of_name), o->all());
    }
    return appendFuncs(std::move(external_funcs_of_name), std::vector<util::sref<Function>>());
}

util::sref<Function> Overloads::queryOrNulIfNonexist(std::string const& name, int param_count) const
{
    util::sref<Overload const> o = _overloadByNameOrNulIfNonexist(name);
    if (o.not_nul()) {
        util::sref<Function> f = o->queryOrNulIfNonexist(param_count);
        if (f.not_nul()) {
            return f;
        }
    }
    if (_external_overloads_or_nul_on_global.not_nul()) {
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

void SymbolTable::refVars(misc::position const& pos, std::vector<std::string> const& vars)
{
    std::for_each(vars.begin()
                , vars.end()
                , [&](std::string const& name)
                  {
                      _markReference(pos, name);
                  });
}

void SymbolTable::defVar(misc::position const& pos, std::string const& name)
{
    auto local_refs = _external_var_refs.find(name);
    if (_external_var_refs.end() != local_refs) {
        error::varRefBeforeDef(pos, local_refs->second, name);
    }
    auto insert_result = _var_defs.insert(std::make_pair(name, pos));
    if (!insert_result.second) {
        error::varAlreadyInLocal(insert_result.first->second, pos, name);
    }
}

void SymbolTable::defFunc(util::sref<Function> func)
{
    util::sref<Function> old_func
            = _overloads.queryOrNulIfNonexist(func->name, func->param_names.size());
    if (old_func.not_nul()) {
        error::funcAlreadyDef(old_func->pos, func->pos, func->name, func->param_names.size());
        return;
    }
    _overloads.declare(func);
}

std::vector<util::sptr<proto::Expression const>> SymbolTable::_mkArgs(
              std::vector<util::sptr<Expression const>> const& args
            , util::sref<proto::Scope> scope)
{
    std::vector<util::sptr<proto::Expression const>> arguments;
    arguments.reserve(args.size());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& expr)
                  {
                      arguments.push_back(expr->compile(scope, util::mkref(*this)));
                  });
    return std::move(arguments);
}

util::sref<proto::Function> SymbolTable::_compileFunction(misc::position const& pos
                                                        , util::sref<Function> func
                                                        , util::sref<proto::Scope> scope)
{
    util::sref<proto::Function> compiled_func = func->compile(scope);
    refVars(pos, func->freeVariables());
    return compiled_func;
}

util::sptr<proto::Expression const> SymbolTable::_compileAsFunctor(
                misc::position const& pos
              , std::string const& name
              , util::sref<proto::Scope> scope
              , std::vector<util::sptr<Expression const>> const& args)
{
    return util::mkptr(new proto::Functor(pos, name, _mkArgs(args, scope)));
}

util::sptr<proto::Expression const> SymbolTable::compileRef(misc::position const& pos
                                                          , std::string const& name
                                                          , util::sref<proto::Scope> scope)
{
    std::vector<util::sref<Function>> funcs = _overloads.allFuncsOfName(name);
    if (!funcs.empty()) {
        if (funcs.size() > 1) {
            error::funcReferenceAmbiguous(pos, name);
        }
        return util::mkptr(new proto::FuncReference(pos, _compileFunction(pos, funcs[0], scope)));
    }
    _markReference(pos, name);
    return util::mkptr(new proto::Reference(pos, name));
}

util::sptr<proto::Expression const> SymbolTable::compileCall(
                    misc::position const& pos
                  , util::sref<proto::Scope> scope
                  , std::string const& name
                  , std::vector<util::sptr<Expression const>> const& args)
{
    util::sref<Function> func = _overloads.queryOrNulIfNonexist(name, args.size());
    if (func.not_nul()) {
        return util::mkptr(
                new proto::Call(pos, _compileFunction(pos, func, scope), _mkArgs(args, scope)));
    }
    _markReference(pos, name);
    return _compileAsFunctor(pos, name, scope, args);
}

util::sref<Function> SymbolTable::queryFunc(misc::position const& pos
                                          , std::string const& name
                                          , int param_count)
{
    util::sref<Function> func = _overloads.queryOrNulIfNonexist(name, param_count);
    if (func.not_nul()) {
        return func;
    }
    error::funcNotDef(pos, name, param_count);
    return util::mkref(_fake_function);
}

std::vector<std::string> SymbolTable::freeVariables() const
{
    std::vector<std::string> result;
    std::for_each(_external_var_refs.begin()
                , _external_var_refs.end()
                , [&](std::pair<std::string, std::list<misc::position>> const& ref)
                  {
                      result.push_back(ref.first);
                  });
    return result;
}

void SymbolTable::_markReference(misc::position const& pos, std::string const& name)
{
    if (_var_defs.end() == _var_defs.find(name)) {
        _external_var_refs[name].push_back(pos);
    }
}

Function SymbolTable::_fake_function(misc::position(0)
                                   , ""
                                   , std::vector<std::string>()
                                   , util::mkptr(new GlobalFilter));
