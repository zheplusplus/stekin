#include "symbol-def-filter.h"
#include "expr-nodes.h"
#include "symbol-table.h"
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
                             , util::sptr<Filter>)
{
    error::forbidDefFunc(pos, name);
}

util::sref<SymbolTable> SymbolDefFilter::getSymbols()
{
    return _symbols;
}
