#include "type.h"
#include "expr-nodes.h"
#include "variable.h"
#include "func-inst-draft.h"
#include "../output/name-mangler.h"
#include "../report/errors.h"

using namespace proto;

static BuiltInPrimitive const BAD_TYPE_DEF("*BAD STEKIN TYPE*", 0);
static BuiltInPrimitive const VOID("void", 0);
static BuiltInPrimitive const BOOL("bool", 1);
static BuiltInPrimitive const INT("int", 4);
static BuiltInPrimitive const FLOAT("float", 8);

util::sref<Type const> const Type::BAD_TYPE(util::mkref(BAD_TYPE_DEF));
util::sref<Type const> const Type::BIT_VOID(util::mkref(VOID));
util::sref<Type const> const Type::BIT_BOOL(util::mkref(BOOL));
util::sref<Type const> const Type::BIT_INT(util::mkref(INT));
util::sref<Type const> const Type::BIT_FLOAT(util::mkref(FLOAT));

bool Type::operator!=(Type const& rhs) const
{
    return !operator==(rhs);
}

bool Type::eqAsBuiltIn(Type const&) const
{
    return false;
}

bool Type::eqAsFuncReference(util::sref<Function>
                           , std::map<std::string, Variable const> const&) const
{
    return false;
}

bool Type::ltAsBuiltIn(Type const&) const
{
    return false;
}

bool Type::ltAsFuncReference(util::sref<Function>
                           , std::map<std::string, Variable const> const&) const
{
    return false;
}

void Type::checkCondType(misc::position const& pos) const
{
    error::condNotBool(pos, name());
}

std::string BuiltInPrimitive::exportedName() const
{
    return output::formType(tname);
}

std::string BuiltInPrimitive::name() const
{
    return tname;
}

bool BuiltInPrimitive::operator==(Type const& rhs) const
{
    return rhs.eqAsBuiltIn(*this);
}

bool BuiltInPrimitive::operator<(Type const& rhs) const
{
    return rhs.ltAsBuiltIn(*this);
}

bool BuiltInPrimitive::eqAsBuiltIn(Type const& lhs) const
{
    return &lhs == this;
}

bool BuiltInPrimitive::ltAsBuiltIn(Type const& lhs) const
{
    return &lhs < this;
}

bool BuiltInPrimitive::ltAsFuncReference(util::sref<Function>
                                       , std::map<std::string, Variable const> const&) const
{
    return false;
}

void BuiltInPrimitive::checkCondType(misc::position const& pos) const
{
    if (&BOOL != this && &BAD_TYPE_DEF != this) {
        Type::checkCondType(pos);
    }
}

util::sref<FuncInstDraft> BuiltInPrimitive::call(misc::position const& call_pos
                                               , int
                                               , int
                                               , std::vector<util::sref<Type const>> const&) const
{
    error::requestVariableNotCallable(call_pos);
    return FuncInstDraft::badDraft();
}
