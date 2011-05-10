#include "built-in.h"
#include "../output/statement-writer.h"

using namespace inst;

void WriteExpr::write() const
{
    output::beginWriteStmt();
    expr->write();
    output::endWriteStmt();
}
