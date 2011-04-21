#include "node-base.h"
#include "scope.h"
#include "function.h"
#include "variable.h"
#include "symbol-table.h"
#include "type.h"
#include "operation.h"

using namespace inst;

operation const* scope::query_binary(misc::pos_type const& pos
                                   , std::string const& op
                                   , util::sref<type const> lhs
                                   , util::sref<type const> rhs) const
{
    return operation::query_binary(pos, op, lhs, rhs);
}

operation const* scope::query_pre_unary(misc::pos_type const& pos
                                      , std::string const& op
                                      , util::sref<type const> rhs) const
{
    return operation::query_pre_unary(pos, op, rhs);
}

void scope::add_stmt(util::sptr<Statement const> stmt)
{
    _Block.add_stmt(std::move(stmt));
}
