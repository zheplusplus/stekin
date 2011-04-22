#include "type.h"
#include "expr-nodes.h"
#include "../output/name-mangler.h"
#include "../report/errors.h"

using namespace inst;

static built_in_primitive const BAD_TYPE_DEF("*BAD STEKIN TYPE*", 0);
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

bool type::eq_as_FuncReference(util::sref<proto::Function>, std::map<std::string, variable const> const&) const
{
    return false;
}

bool type::lt_as_built_in(type const&) const
{
    return false;
}

bool type::lt_as_FuncReference(util::sref<proto::Function>, std::map<std::string, variable const> const&) const
{
    return false;
}

void type::check_condition_type(misc::position const& pos) const
{
    error::condNotBool(pos, name());
}

std::string built_in_primitive::exported_name() const
{
    return output::form_type(tname);
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

bool built_in_primitive::lt_as_FuncReference(util::sref<proto::Function>
                                            , std::map<std::string, variable const> const&) const
{
    return false;
}

void built_in_primitive::check_condition_type(misc::position const& pos) const
{
    if (&BOOL != this && &BAD_TYPE_DEF != this) {
        type::check_condition_type(pos);
    }
}

util::sptr<inst::Expression const> built_in_primitive::call_func(
                  misc::position const& call_pos
                , int
                , int
                , std::vector<util::sref<inst::type const>> const&
                , std::vector<util::sptr<Expression const>>) const
{
    error::request_variable_not_callable(call_pos);
    return std::move(util::mkptr(new BoolLiteral(false)));
}
