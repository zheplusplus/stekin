#include "built-in.h"
#include "../instance/built-in.h"

using namespace proto;

util::sptr<inst::Expression const> WriteExpr::inst(util::sref<inst::SymbolTable const> st)
{
    return util::mkptr(new inst::WriteExpr(ref.inst(st)));
}
