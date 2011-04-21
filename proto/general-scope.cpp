#include "general-scope.h"
#include "expr-nodes.h"
#include "stmt-nodes.h"
#include "function.h"
#include "sub-scope.h"

using namespace proto;

util::sptr<Expression const> general_scope::make_ref(misc::position const& pos, std::string const& name)
{
    return std::move(_symbols.ref_var(pos, name));
}

util::sptr<Expression const> general_scope::make_call(misc::position const& pos
                                                   , std::string const& name
                                                   , std::vector<util::sptr<Expression const>> args) const
{
    return std::move(_symbols.query_call(pos, name, std::move(args)));
}

util::sptr<Expression const> general_scope::make_func_reference(misc::position const& pos
                                                             , std::string const& name
                                                             , int param_count) const
{
    return std::move(util::mkptr(new func_reference(pos, _symbols.query_func(pos, name, param_count))));
}

void general_scope::def_var(misc::position const& pos, std::string const& name)
{
    _symbols.def_var(pos, name);
}

util::sref<Function> general_scope::declare(misc::position const& pos
                                          , std::string const& name
                                          , std::vector<std::string> const& param_names
                                          , bool hint_void_return)
{
    return _symbols.def_func(pos, name, param_names, hint_void_return);
}

util::sptr<scope> general_scope::create_branch_scope()
{
    return std::move(util::mkmptr(new sub_scope(util::mkref(_symbols))));
}
