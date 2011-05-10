#include "built-in.h"
#include "variable.h"
#include "func-inst-draft.h"
#include "type.h"
#include "../instance/built-in.h"

using namespace proto;

util::sref<Type const> WriteExpr::type(util::sref<SymbolTable const>)
{
    return Type::BIT_VOID;
}

util::sptr<inst::Expression const> WriteExpr::inst(util::sref<SymbolTable const> st)
{
    return util::mkptr(new inst::WriteExpr(ref.inst(st)));
}
