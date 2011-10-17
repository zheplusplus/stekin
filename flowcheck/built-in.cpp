#include <proto/built-in.h>

#include "built-in.h"
#include "expr-nodes.h"
#include "stmt-nodes.h"
#include "func-body-filter.h"
#include "symbol-def-filter.h"

using namespace flchk;

util::sptr<proto::Expression const> WriterExpr::compile(util::sref<proto::Block>
                                                      , util::sref<SymbolTable>) const
{
    return util::mkptr(new proto::WriterExpr);
}

std::string WriterExpr::typeName() const
{
    return "Stekin Built-in Write";
}

bool WriterExpr::isLiteral() const
{
    return false;
}

bool WriterExpr::boolValue() const
{
    return false;
}

util::sptr<Expression const> WriterExpr::fold() const
{
    return util::mkptr(new WriterExpr);
}

util::sptr<Filter> WriterFunction::_mkBody(util::sref<SymbolTable> global_symbols)
{
    util::sptr<Filter> body(new FuncBodyFilter(global_symbols));
    body->addArith(misc::position(), util::mkptr(new WriterExpr));
    return std::move(body);
}

util::sptr<Filter> SelectorFunction::_mkBody(util::sref<SymbolTable> global_symbols)
{
    struct BuiltInRef
        : public Reference
    {
        explicit BuiltInRef(std::string const& name)
            : Reference(misc::position(), name)
        {}
    };

    util::sptr<Filter> body(new FuncBodyFilter(global_symbols));
    util::sptr<Filter> ret_consq(new SymbolDefFilter(global_symbols));
    ret_consq->addReturn(misc::position(), util::mkptr(new BuiltInRef("c")));
    body->addBranch(misc::position(), util::mkptr(new BuiltInRef("p")), std::move(ret_consq));
    body->addReturn(misc::position(), util::mkptr(new BuiltInRef("a")));
    return std::move(body);
}
