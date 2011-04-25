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
                 , util::sref<SymbolTable const> ext_symbols
                 , bool func_hint_void_return)
    : GeneralScope(ext_symbols)
    , pos(ps)
    , name(name)
    , param_names(params)
    , hint_void_return(func_hint_void_return)
{
    _fillParamNames();
}

void Function::_fillParamNames()
{
    std::for_each(param_names.begin()
                , param_names.end()
                , [&](std::string const& param_name)
                  {
                      _symbols.defVar(pos, param_name);
                  });
}

util::sref<inst::Function> Function::inst(
                                        misc::position const& pos
                                      , util::sref<inst::Scope> ext_scope
                                      , std::vector<util::sref<inst::Type const>> const& arg_types)
{
    return inst(ext_scope->level(), bindExternalVars(pos, ext_scope), arg_types);
}

util::sref<inst::Function> Function::inst(
                                        int level
                                      , std::map<std::string, inst::Variable const> const& ext_vars
                                      , std::vector<util::sref<inst::Type const>> const& arg_types)
{
    auto find_result = _instance_cache.find(InstanceInfo(ext_vars, arg_types));
    if (_instance_cache.end() != find_result) {
        util::sref<inst::Function> instance = find_result->second;
        while (!instance->isReturnTypeResolved() && instance->hasMorePath()) {
            instance->instNextPath();
        }
        if (!instance->isReturnTypeResolved()) {
            error::returnTypeUnresolvable(name, arg_types.size());
        }
        return instance;
    }

    std::list<inst::ArgNameTypeRec> args;
    for (unsigned i = 0; i < arg_types.size(); ++i) {
        args.push_back(inst::ArgNameTypeRec(param_names[i], arg_types[i]));
    }
    util::sref<inst::Function> instance = inst::Function::createInstance(level
                                                                       , args
                                                                       , ext_vars
                                                                       , hint_void_return);
    _instance_cache.insert(std::make_pair(InstanceInfo(ext_vars, arg_types), instance));

    BlockMediate body_mediate(_block.getStmts(), instance);
    instance->instNextPath();
    instance->addStmt(std::move(body_mediate.inst(instance)));
    return instance;
}

std::map<std::string, inst::Variable const> Function::bindExternalVars(
                                                  misc::position const& pos
                                                , util::sref<inst::Scope const> ext_scope) const
{
    return _symbols.bindExternalVars(pos, ext_scope);
}

std::vector<std::string> Function::freeVariables() const
{
    return _symbols.freeVariables();
}

bool Function::InstanceInfo::operator<(Function::InstanceInfo const& rhs) const
{
    if (arg_types != rhs.arg_types) {
        return arg_types < rhs.arg_types;
    }
    return util::map_less(ext_vars, rhs.ext_vars);
}
