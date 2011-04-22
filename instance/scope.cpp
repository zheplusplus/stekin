#include "node-base.h"
#include "scope.h"
#include "function.h"
#include "variable.h"
#include "symbol-table.h"
#include "type.h"
#include "operation.h"

using namespace inst;

operation const* Scope::query_binary(misc::position const& pos
                                   , std::string const& op
                                   , util::sref<type const> lhs
                                   , util::sref<type const> rhs) const
{
    return operation::query_binary(pos, op, lhs, rhs);
}

operation const* Scope::query_pre_unary(misc::position const& pos
                                      , std::string const& op
                                      , util::sref<type const> rhs) const
{
    return operation::query_pre_unary(pos, op, rhs);
}

void Scope::addStmt(util::sptr<Statement const> stmt)
{
    _block.addStmt(std::move(stmt));
}
