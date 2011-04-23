#include "node-base.h"
#include "scope.h"
#include "function.h"
#include "variable.h"
#include "symbol-table.h"
#include "type.h"
#include "operation.h"

using namespace inst;

Operation const* Scope::queryBinary(misc::position const& pos
                                  , std::string const& op
                                  , util::sref<Type const> lhs
                                  , util::sref<Type const> rhs) const
{
    return Operation::queryBinary(pos, op, lhs, rhs);
}

Operation const* Scope::queryPreUnary(misc::position const& pos
                                    , std::string const& op
                                    , util::sref<Type const> rhs) const
{
    return Operation::queryPreUnary(pos, op, rhs);
}

void Scope::addStmt(util::sptr<Statement const> stmt)
{
    _block.addStmt(std::move(stmt));
}
