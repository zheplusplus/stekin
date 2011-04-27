#include "symbol-def-filter.h"
#include "node-base.h"
#include "expr-nodes.h"
#include "function.h"
#include "../proto/node-base.h"
#include "../report/errors.h"

using namespace flchk;

void SymbolDefFilter::defVar(misc::position const& pos
                           , std::string const& name
                           , util::sptr<Expression const>)
{
    error::forbidDefVar(pos, name);
}

void SymbolDefFilter::_defFunc(misc::position const& pos
                             , std::string const& name
                             , std::vector<std::string> const&
                             , Accumulator)
{
    error::forbidDefFunc(pos, name);
}

util::sptr<Expression const> SymbolDefFilter::makeRef(misc::position const& pos
                                                    , std::string const& name)
{
    return std::move(util::mkptr(new Reference(pos, name)));
}

util::sptr<Expression const> SymbolDefFilter::makeCall(
                    misc::position const& pos
                  , std::string const& name
                  , std::vector<util::sptr<Expression const>> args)
{
    return std::move(util::mkptr(new Call(pos, name, std::move(args))));
}

util::sptr<Expression const> SymbolDefFilter::makeFuncReference(misc::position const& pos
                                                              , std::string const& name
                                                              , int param_count)
{
    return std::move(util::mkptr(new FuncReference(pos, name, param_count)));
}
