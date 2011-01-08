#include "scope.h"
#include "function.h"
#include "variable.h"
#include "symbol-table.h"
#include "type.h"
#include "operation.h"

using namespace inst;

void scope::set_return_type(misc::pos_type const& pos, type const* type) const
{
    _func->set_return_type(pos, type);
}

variable scope::def_var(misc::pos_type const& pos, type const* vtype, std::string const& name) const
{
    return _symbols->def_var(pos, vtype, name);
}

variable scope::query_var(misc::pos_type const& pos, std::string const& name) const
{
    return _symbols->query_var(pos, name);
}

operation const* scope::query_binary(misc::pos_type const& pos
                                   , std::string const& op
                                   , type const* lhs
                                   , type const* rhs) const
{
    return operation::query_binary(pos, op, lhs, rhs);
}

operation const* scope::query_pre_unary(misc::pos_type const& pos, std::string const& op, type const* rhs) const
{
    return operation::query_pre_unary(pos, op, rhs);
}

void scope::add_stmt(util::sptr<stmt_base const>&& stmt)
{
    _block.add_stmt(std::move(stmt));
}
