#include "type.h"
#include "expr-nodes.h"
#include "variable.h"
#include "func-inst-draft.h"
#include "../misc/platform.h"
#include "../report/errors.h"

using namespace proto;

namespace {

    struct BadType
        : public BuiltInPrimitive
    {
        BadType()
            : BuiltInPrimitive("*BAD STEKIN TYPE*", 0)
        {}

        util::sptr<inst::Type const> makeInstType() const
        {
            return util::mkptr(new inst::VoidPrimitive);
        }
    };
    BadType BAD_TYPE_DEF;

    struct VoidPrimitive
        : public BuiltInPrimitive
    {
        VoidPrimitive()
            : BuiltInPrimitive("void", 0)
        {}

        util::sptr<inst::Type const> makeInstType() const
        {
            return util::mkptr(new inst::VoidPrimitive);
        }
    };
    VoidPrimitive VOID;

    struct BoolPrimitive
        : public BuiltInPrimitive
    {
        BoolPrimitive()
            : BuiltInPrimitive("bool", platform::BOOL_SIZE)
        {}

        util::sptr<inst::Type const> makeInstType() const
        {
            return util::mkptr(new inst::BoolPrimitive);
        }
    };
    BoolPrimitive BOOL;

    struct IntPrimitive
        : public BuiltInPrimitive
    {
        IntPrimitive()
            : BuiltInPrimitive("int", platform::INT_SIZE)
        {}

        util::sptr<inst::Type const> makeInstType() const
        {
            return util::mkptr(new inst::IntPrimitive);
        }
    };
    IntPrimitive INT;

    struct FloatPrimitive
        : public BuiltInPrimitive
    {
        FloatPrimitive()
            : BuiltInPrimitive("float", platform::FLOAT_SIZE)
        {}

        util::sptr<inst::Type const> makeInstType() const
        {
            return util::mkptr(new inst::FloatPrimitive);
        }
    };
    FloatPrimitive FLOAT;

}

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
