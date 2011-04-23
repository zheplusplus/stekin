#include "test-common.h"
#include "../../misc/platform.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"

using namespace test;

inst::BuiltInPrimitive const test::WORD("word", platform::WORD_LENGTH_INBYTE);
inst::BuiltInPrimitive const test::DWORD("dword", platform::WORD_LENGTH_INBYTE * 2);
inst::BuiltInPrimitive const test::HALFWORD("halfword", platform::WORD_LENGTH_INBYTE / 2);

void InstanceTest::SetUp()
{
    clearErr();
}

std::ostream& operator<<(std::ostream& os, inst::Variable const& var)
{
    os << "variable pos=" << var.def_pos
       << " type=" << var.type->name()
       << " offset=" << var.stack_offset
       << " level=" << var.level;
    return os;
}

std::ostream& operator<<(std::ostream& os, util::sref<inst::Type const> type)
{
    return os << type->name();
}
