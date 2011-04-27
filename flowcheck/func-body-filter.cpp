#include "func-body-filter.h"
#include "node-base.h"
#include "expr-nodes.h"
#include "function.h"
#include "../proto/node-base.h"

using namespace flchk;

void FuncBodyFilter::defVar(misc::position const& pos
                          , std::string const& name
                          , util::sptr<Expression const> init)
{
    _accumulator.defVar(pos, name, std::move(init->fold()));
}

void FuncBodyFilter::_defFunc(misc::position const& pos
                            , std::string const& name
                            , std::vector<std::string> const& param_names
                            , Accumulator body)
{
    _accumulator.defFunc(pos, name, param_names, std::move(body));
}

util::sptr<Expression const> FuncBodyFilter::makeRef(misc::position const& pos
                                                   , std::string const& name)
{
    return std::move(util::mkptr(new Reference(pos, name)));
}

util::sptr<Expression const> FuncBodyFilter::makeCall(
                    misc::position const& pos
                  , std::string const& name
                  , std::vector<util::sptr<Expression const>> args)
{
    return std::move(util::mkptr(new Call(pos, name, std::move(args))));
}

util::sptr<Expression const> FuncBodyFilter::makeFuncReference(misc::position const& pos
                                                             , std::string const& name
                                                             , int param_count)
{
    return std::move(util::mkptr(new FuncReference(pos, name, param_count)));
}
