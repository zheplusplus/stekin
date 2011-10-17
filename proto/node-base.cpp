#include <instance/node-base.h>

#include "node-base.h"

using namespace proto;

util::sref<Type const> Expression::typeAsPipe(util::sref<SymbolTable const> st
                                            , util::sref<ListContext const>
                                            , misc::trace& trace) const
{
    return type(st, trace);
}

util::sptr<inst::Expression const> Expression::instAsPipe(util::sref<SymbolTable const> st
                                                        , util::sref<ListContext const>
                                                        , misc::trace& trace) const
{
    return inst(st, trace);
}
