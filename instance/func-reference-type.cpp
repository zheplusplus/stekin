#include "func-reference-type.h"
#include "../util/string.h"
#include "../util/map-compare.h"
#include "../proto/function.h"
#include "../proto/node-base.h"

using namespace inst;

std::string func_reference_type::name() const
{
    return "Function reference [ "
         + _func_proto->name
         + " with "
         + util::str(int(_func_proto->param_names.size()))
         + " parameters ]";
}

bool func_reference_type::operator==(type const& rhs) const
{
    return rhs.eq_as_func_reference(_func_proto, context_references);
}

bool func_reference_type::operator<(type const& rhs) const
{
    return rhs.lt_as_func_reference(_func_proto, context_references);
}

bool func_reference_type::eq_as_func_reference(util::sref<proto::function> lhs_func
                                             , std::map<std::string, variable const> const& rhs_cr) const
{
    return _func_proto.id() == lhs_func.id() && context_references == rhs_cr;
}

bool func_reference_type::lt_as_func_reference(util::sref<proto::function> lhs_func
                                             , std::map<std::string, variable const> const& rhs_cr) const
{
    if (_func_proto.id() == lhs_func.id()) {
        return util::map_less(context_references, rhs_cr);
    }
    return _func_proto.id() < lhs_func.id();
}

bool func_reference_type::lt_as_built_in(type const&) const
{
    return true;
}

util::sref<proto::function> func_reference_type::get_func_proto() const
{
    return _func_proto;
}
