#include <misc/platform.h>
#include <report/errors.h>

#include "type.h"
#include "expr-nodes.h"
#include "variable.h"
#include "func-inst-draft.h"

using namespace proto;

namespace {

    struct BuiltInPrimitive
        : public Type
    {
        BuiltInPrimitive(std::string const& n, int size)
            : Type(size)
            , tname(n)
        {}

        std::string name() const
        {
            return tname;
        }

        void checkCondType(misc::position const& pos) const
        {
            if (BIT_BOOL != util::mkref(*this) && BAD_TYPE != util::mkref(*this)) {
                Type::checkCondType(pos);
            }
        }

        std::string const tname;
    };

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

bool Type::operator==(Type const& rhs) const
{
    return this == &rhs;
}

bool Type::operator<(Type const& rhs) const
{
    return this < &rhs;
}

util::sref<FuncInstDraft> Type::call(int
                                   , int
                                   , std::vector<util::sref<Type const>> const&
                                   , misc::trace& trace) const
{
    error::requestVariableNotCallable(trace.top());
    return FuncInstDraft::badDraft();
}

void Type::checkCondType(misc::position const& pos) const
{
    error::condNotBool(pos, name());
}

std::vector<int> Type::resEntries(int) const
{
    return std::vector<int>();
}

util::sref<Type const> Type::memberCallType(std::string const& call_name
                                          , std::vector<util::sref<Type const>> const&
                                          , misc::trace& trace) const
{
    error::memberCallNotFound(trace.top(), name(), call_name);
    return BAD_TYPE;
}
