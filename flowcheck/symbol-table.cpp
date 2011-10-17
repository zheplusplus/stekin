#include <algorithm>

#include <proto/variable.h>
#include <proto/expr-nodes.h>
#include <util/vector-append.h>
#include <report/errors.h>

#include "symbol-table.h"
#include "function.h"
#include "expr-nodes.h"
#include "global-filter.h"

using namespace flchk;

util::sref<Function> Overloads::Overload::queryOrNulIfNonexist(int param_count) const
{
    auto func = _funcs.find(param_count);
    if (_funcs.end() == func) {
        return util::sref<Function>(nullptr);
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
        return util::refs_append(std::move(external_funcs_of_name), o->all());
    }
    return external_funcs_of_name;
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
    return util::sref<Function>(nullptr);
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
    return util::sref<Overload const>(nullptr);
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
            , util::sref<proto::Block> block)
{
    std::vector<util::sptr<proto::Expression const>> arguments;
    arguments.reserve(args.size());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& expr)
                  {
                      arguments.push_back(expr->compile(block, util::mkref(*this)));
                  });
    return std::move(arguments);
}

util::sref<proto::Function> SymbolTable::_compileFunction(misc::position const& pos
                                                        , util::sref<Function> func
                                                        , util::sref<proto::Block> block)
{
    util::sref<proto::Function> compiled_func = func->compile(block);
    refVars(pos, func->freeVariables());
    return compiled_func;
}

util::sptr<proto::Expression const> SymbolTable::_compileAsFunctor(
                misc::position const& pos
              , std::string const& name
              , util::sref<proto::Block> block
              , std::vector<util::sptr<Expression const>> const& args)
{
    return util::mkptr(new proto::Functor(pos, name, _mkArgs(args, block)));
}

util::sptr<proto::Expression const> SymbolTable::compileRef(misc::position const& pos
                                                          , std::string const& name
                                                          , util::sref<proto::Block> block)
{
    std::vector<util::sref<Function>> funcs = _overloads.allFuncsOfName(name);
    if (!funcs.empty()) {
        if (funcs.size() > 1) {
            error::funcReferenceAmbiguous(pos, name);
        }
        return util::mkptr(new proto::FuncReference(pos, _compileFunction(pos, funcs[0], block)));
    }
    _markReference(pos, name);
    return util::mkptr(new proto::Reference(pos, name));
}

util::sptr<proto::Expression const> SymbolTable::compileCall(
                    misc::position const& pos
                  , util::sref<proto::Block> block
                  , std::string const& name
                  , std::vector<util::sptr<Expression const>> const& args)
{
    util::sref<Function> func = _overloads.queryOrNulIfNonexist(name, args.size());
    if (func.not_nul()) {
        return util::mkptr(
                new proto::Call(pos, _compileFunction(pos, func, block), _mkArgs(args, block)));
    }
    _markReference(pos, name);
    return _compileAsFunctor(pos, name, block, args);
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

Function SymbolTable::_fake_function(misc::position()
                                   , ""
                                   , std::vector<std::string>()
                                   , util::mkptr(new GlobalFilter));
