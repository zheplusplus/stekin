#include "general-scope.h"
#include "expr-nodes.h"
#include "stmt-nodes.h"
#include "function.h"
#include "sub-scope.h"

using namespace proto;

util::sptr<Expression const> GeneralScope::makeRef(misc::position const& pos, std::string const& name)
{
    return std::move(_symbols.refVar(pos, name));
}

util::sptr<Expression const> GeneralScope::makeCall(misc::position const& pos
                                                  , std::string const& name
                                                  , std::vector<util::sptr<Expression const>> args)
                                                const
{
    return std::move(_symbols.queryCall(pos, name, std::move(args)));
}

util::sptr<Expression const> GeneralScope::makeFuncReference(misc::position const& pos
                                                           , std::string const& name
                                                           , int param_count) const
{
    return std::move(util::mkptr(new FuncReference(pos
                                                 , _symbols.queryFunc(pos, name, param_count))));
}

void GeneralScope::defVar(misc::position const& pos, std::string const& name)
{
    _symbols.defVar(pos, name);
}

util::sref<Function> GeneralScope::declare(misc::position const& pos
                                         , std::string const& name
                                         , std::vector<std::string> const& param_names
                                         , bool hint_void_return)
{
    return _symbols.defFunc(pos, name, param_names, hint_void_return);
}

util::sptr<Scope> GeneralScope::createBranchScope()
{
    return std::move(util::mkmptr(new SubScope(util::mkref(_symbols))));
}
