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
    _accumulator.defVar(pos, util::mkref(_symbols), name, std::move(init->fold()));
}

void FuncBodyFilter::_defFunc(misc::position const& pos
                            , std::string const& name
                            , std::vector<std::string> const& param_names
                            , util::sptr<Filter> body)
{
    _symbols.defFunc(_accumulator.defFunc(pos
                                        , name
                                        , param_names
                                        , std::move(body)));
}

util::sref<SymbolTable> FuncBodyFilter::getSymbols()
{
    return util::mkref(_symbols);
}
