#include <algorithm>

#include <output/built-in-writer.h>

#include "built-in.h"

using namespace inst;

void WriterExpr::write() const
{
    output::beginWriterStmt();
    expr->write();
    output::endWriterStmt();
}
