#include "built-in.h"
#include "variable.h"
#include "../instance/built-in.h"
#include "../instance/type.h"

using namespace proto;

util::sref<inst::Type const> WriteExpr::type(util::sref<SymbolTable const>)
{
    return inst::Type::BIT_VOID;
}

util::sptr<inst::Expression const> WriteExpr::inst(util::sref<SymbolTable const> st)
{
    return util::mkptr(new inst::WriteExpr(ref.inst(st)));
}
