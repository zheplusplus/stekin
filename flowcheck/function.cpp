#include <algorithm>

#include "function.h"
#include "node-base.h"
#include "filter.h"
#include "symbol-table.h"
#include "../proto/node-base.h"
#include "../proto/function.h"

using namespace flchk;

util::sref<proto::Function> Function::compile(util::sref<proto::Scope> scope)
{
    if (!bool(_func_proto)) {
        _func_proto = scope->declare(pos, name, param_names, _body->bodyContainsVoidReturn());
        std::for_each(param_names.begin()
                    , param_names.end()
                    , [&](std::string const& param)
                      {
                          _body->getSymbols()->defVar(pos, param);
                      });
        _body->deliver().compile(_func_proto);
        _func_proto->setFreeVariables(_body->getSymbols()->freeVariables());
    }
    return _func_proto;
}

std::vector<std::string> Function::freeVariables() const
{
    return _body->getSymbols()->freeVariables();
}
