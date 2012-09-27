#include <instance/built-in.h>

#include "built-in.h"
#include "variable.h"
#include "func-inst-draft.h"
#include "type.h"

using namespace proto;

util::sref<Type const> WriterExpr::type(util::sref<SymbolTable const>, misc::trace&) const
{
    return Type::s_void();
}

util::sptr<inst::Expression const> WriterExpr::inst(util::sref<SymbolTable const> st
                                                  , misc::trace& trace) const
{
    return util::mkptr(new inst::WriterExpr(ref.inst(st, trace)));
}
