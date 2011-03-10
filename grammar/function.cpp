#include "function.h"
#include "node-base.h"
#include "../proto/node-base.h"
#include "../proto/function.h"

using namespace grammar;

util::sref<proto::function> function::declare(util::sref<proto::scope> scope) const
{
    return scope->decl_func(pos, name, param_names);
}

void function::compile(util::sref<proto::function> func) const
{
    body.compile(func);
}
