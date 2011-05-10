#include <algorithm>

#include "function.h"
#include "node-base.h"
#include "filter.h"
#include "symbol-table.h"
#include "../proto/node-base.h"
#include "../proto/function.h"
#include "../proto/variable.h"

using namespace flchk;

util::sref<proto::Function> Function::compile(util::sref<proto::Scope> scope)
{
    if (_func_proto_or_nul_if_not_compiled.nul()) {
        _func_proto_or_nul_if_not_compiled
                = scope->declare(pos, name, param_names, _body->hintReturnVoid());
        std::for_each(param_names.begin()
                    , param_names.end()
                    , [&](std::string const& param)
                      {
                          _body->getSymbols()->defVar(pos, param);
                      });
        _body->compile(_func_proto_or_nul_if_not_compiled);
        _func_proto_or_nul_if_not_compiled->setFreeVariables(_body->getSymbols()->freeVariables());
    }
    return _func_proto_or_nul_if_not_compiled;
}

std::vector<std::string> Function::freeVariables() const
{
    return _body->getSymbols()->freeVariables();
}
