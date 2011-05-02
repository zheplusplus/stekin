#include "built-in.h"
#include "type.h"
#include "../output/statement-writer.h"

using namespace inst;

util::sref<Type const> WriteExpr::typeof() const
{
    return Type::BIT_VOID;
}

void WriteExpr::write() const
{
    output::beginWriteStmt();
    expr->write();
    output::endWriteStmt();
}
