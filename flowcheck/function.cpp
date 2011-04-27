#include "function.h"
#include "node-base.h"
#include "../proto/node-base.h"
#include "../proto/function.h"

using namespace flchk;

void Function::compile(util::sref<proto::Scope> scope) const
{
    body.compile(scope->declare(pos, name, param_names, contains_void_return));
}
