#include "type.h"
#include "../report/errors.h"
#include "../util/string.h"
#include "../util/map-compare.h"
#include "../proto/function.h"
#include "../proto/node-base.h"

using namespace inst;

static built_in_primitive const BAD_TYPE_DEF("*BAD STACKENING TYPE*", 0);
static built_in_primitive const VOID("void", 0);
static built_in_primitive const BOOL("bool", 1);
static built_in_primitive const INT("int", 4);
static built_in_primitive const FLOAT("float", 8);

util::sref<type const> const type::BAD_TYPE(util::mkref(BAD_TYPE_DEF));
util::sref<type const> const type::BIT_VOID(util::mkref(VOID));
util::sref<type const> const type::BIT_BOOL(util::mkref(BOOL));
util::sref<type const> const type::BIT_INT(util::mkref(INT));
util::sref<type const> const type::BIT_FLOAT(util::mkref(FLOAT));

bool type::operator!=(type const& rhs) const
{
    return !operator==(rhs);
}

bool type::eq_as_built_in(type const&) const
{
    return false;
}

bool type::eq_as_func_reference(util::sref<proto::function>
                              , std::map<std::string, variable const> const&) const
{
    return false;
}

bool type::lt_as_built_in(type const&) const
{
    return false;
}

bool type::lt_as_func_reference(util::sref<proto::function>
                              , std::map<std::string, variable const> const&) const
{
    return false;
}

std::string built_in_primitive::name() const
{
    return tname;
}

bool built_in_primitive::operator==(type const& rhs) const
{
    return rhs.eq_as_built_in(*this);
}

bool built_in_primitive::operator<(type const& rhs) const
{
    return rhs.lt_as_built_in(*this);
}

bool built_in_primitive::eq_as_built_in(type const& lhs) const
{
    return &lhs == this;
}

bool built_in_primitive::lt_as_built_in(type const& lhs) const
{
    return &lhs < this;
}

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

util::sref<proto::function> func_reference_type::get_func_proto() const
{
    return _func_proto;
}

void inst::check_condition_type(misc::pos_type const& pos, util::sref<type const> t)
{
    if (type::BIT_BOOL != t && type::BAD_TYPE != t) {
        error::cond_not_bool(pos, t->name());
    }
}
