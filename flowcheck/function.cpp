#include "function.h"
#include "node-base.h"
#include "../proto/node-base.h"
#include "../proto/scope.h"

using namespace flchk;

void function::compile(util::sref<proto::scope> scope) const
{
    body.compile(scope);
}

util::sref<proto::function> function::declare(util::sref<proto::scope> scope) const
{
    return scope->declare(pos, name, param_names, contains_void_return);
}
