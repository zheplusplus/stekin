#include "node-base.h"
#include "scope.h"
#include "function.h"
#include "variable.h"
#include "symbol-table.h"
#include "type.h"

using namespace inst;

void Scope::_addStmt(util::sptr<Statement const> stmt)
{
    _block.addStmt(std::move(stmt));
}
