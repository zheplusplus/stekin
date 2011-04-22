#include "sub-scope.h"
#include "expr-nodes.h"
#include "symbol-table.h"
#include "function.h"
#include "../report/errors.h"

using namespace proto;

util::sptr<Expression const> SubScope::makeRef(misc::position const& pos, std::string const& name)
{
    _symbols->ref_var(pos, name);
    return std::move(util::mkptr(new Reference(pos, name)));
}

util::sptr<Expression const> SubScope::makeCall(misc::position const& pos
                                               , std::string const& name
                                               , std::vector<util::sptr<Expression const>> args) const
{
    return std::move(_symbols->query_call(pos, name, std::move(args)));
}

util::sptr<Expression const> SubScope::makeFuncReference(misc::position const& pos
                                                         , std::string const& name
                                                         , int param_count) const
{
    return std::move(util::mkptr(new FuncReference(pos, _symbols->query_func(pos, name, param_count))));
}

void SubScope::defVar(misc::position const&, std::string const&) {}

util::sref<Function> SubScope::declare(misc::position const&
                                      , std::string const&
                                      , std::vector<std::string> const&
                                      , bool)
{
    return util::sref<Function>(NULL);
}

util::sptr<Scope> SubScope::createBranchScope()
{
    return std::move(util::mkmptr(new SubScope(_symbols)));
}
