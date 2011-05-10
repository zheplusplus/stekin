#include <algorithm>
#include <list>

#include "function.h"
#include "func-inst-draft.h"
#include "symbol-table.h"
#include "expr-nodes.h"
#include "stmt-nodes.h"
#include "variable.h"
#include "../instance/stmt-nodes.h"
#include "../util/map-compare.h"
#include "../report/errors.h"

using namespace proto;

util::sref<FuncInstDraft> Function::inst(misc::position const& pos
                                       , util::sref<SymbolTable const> ext_st
                                       , std::vector<util::sref<Type const>> const& arg_types)
{
    return inst(ext_st->level, bindExternalVars(pos, ext_st), arg_types);
}

static util::sref<FuncInstDraft> instanceAlreadyDoneOrNulIfNonexist(
            std::map<Function::InstanceInfo, util::sref<FuncInstDraft>> const& instance_cache
          , std::map<std::string, Variable const> const& ext_vars
          , std::vector<util::sref<Type const>> const& arg_types
          , std::string const& name)
{
    auto find_result = instance_cache.find(Function::InstanceInfo(ext_vars, arg_types));
    if (instance_cache.end() == find_result) {
        return util::sref<FuncInstDraft>(NULL);
    }
    util::sref<FuncInstDraft> instance = find_result->second;
    while (!instance->isReturnTypeResolved() && instance->hasMorePath()) {
        instance->instNextPath();
    }
    if (!instance->isReturnTypeResolved()) {
        error::returnTypeUnresolvable(name, arg_types.size());
    }
    return instance;
}

std::list<ArgNameTypeRec> makeArgInfo(std::vector<std::string> const& param_names
                                    , std::vector<util::sref<Type const>> const& arg_types)
{
    std::list<ArgNameTypeRec> args;
    for (unsigned i = 0; i < arg_types.size(); ++i) {
        args.push_back(ArgNameTypeRec(param_names[i], arg_types[i]));
    }
    return args;
}

util::sref<FuncInstDraft> Function::inst(int level
                                       , std::map<std::string, Variable const> const& ext_vars
                                       , std::vector<util::sref<Type const>> const& arg_types)
{
    util::sref<FuncInstDraft> result_inst = instanceAlreadyDoneOrNulIfNonexist(_instance_cache
                                                                             , ext_vars
                                                                             , arg_types
                                                                             , name);
    if (result_inst.not_nul()) {
        return result_inst;
    }

    SymbolTable st(level, makeArgInfo(param_names, arg_types), ext_vars);
    util::sref<FuncInstDraft> new_inst = FuncInstDraft::create(util::mkref(st), hint_void_return);
    _instance_cache.insert(std::make_pair(InstanceInfo(ext_vars, arg_types), new_inst));
    new_inst->instantiate(*_block);
    return new_inst;
}

std::map<std::string, Variable const> Function::bindExternalVars(
                                                  misc::position const& pos
                                                , util::sref<SymbolTable const> ext_st) const
{
    std::map<std::string, Variable const> result;
    std::for_each(_free_variables.begin()
                , _free_variables.end()
                , [&](std::string const& var_name)
                  {
                      result.insert(std::make_pair(var_name, ext_st->queryVar(pos, var_name)));
                  });
    return result;
}

void Function::setFreeVariables(std::vector<std::string> const& free_vars)
{
    _free_variables = free_vars;
}

bool Function::InstanceInfo::operator<(Function::InstanceInfo const& rhs) const
{
    if (arg_types != rhs.arg_types) {
        return arg_types < rhs.arg_types;
    }
    return util::map_less(ext_vars, rhs.ext_vars);
}
