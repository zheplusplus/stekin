#include "built-in.h"
#include "../instance/built-in.h"

using namespace proto;

util::sptr<inst::Expression const> WriteExpr::inst(util::sref<inst::Scope> scope) const
{
    return util::mkptr(new inst::WriteExpr(ref.inst(scope)));
}
