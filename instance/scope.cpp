#include "scope.h"
#include "function.h"
#include "variable.h"
#include "instance-symbols.h"
#include "type.h"
#include "operation.h"

using namespace inst;

void scope::set_return_type(int lineno, type const* type) const
{
    _func->set_return_type(lineno, type);
}

variable scope::def_var(int lineno, type const* vtype, std::string const& name) const
{
    return _symbols->def_var(lineno, vtype, name);
}

variable scope::query_var(int lineno, std::string const& name) const
{
    return _symbols->query_var(lineno, name);
}

operation const* scope::query_binary(int lineno, std::string const& op, type const* lhs, type const* rhs) const
{
    return operation::query_binary(lineno, op, lhs, rhs);
}

operation const* scope::query_pre_unary(int lineno, std::string const& op, type const* rhs) const
{
    return operation::query_pre_unary(lineno, op, rhs);
}

inst_symbols* scope::get_symbols() const
{
    return _symbols;
}
