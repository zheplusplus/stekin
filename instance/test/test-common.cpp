#include "test-common.h"
#include "../../misc/platform.h"
#include "../../test/phony-errors.h"

using namespace test;

inst::built_in_primitive const test::WORD("word", platform::WORD_LENGTH_INBYTE);
inst::built_in_primitive const test::DWORD("dword", platform::WORD_LENGTH_INBYTE * 2);
inst::built_in_primitive const test::HALFWORD("halfword", platform::WORD_LENGTH_INBYTE / 2);

void instance_test::SetUp()
{
    clear_err();
}

std::ostream& operator<<(std::ostream& os, inst::variable const& var)
{
    os << "variable pos=" << var.def_pos
       << " type=" << var.vtype->name()
       << " offset=" << var.stack_offset
       << " level=" << var.level;
    return os;
}

std::ostream& operator<<(std::ostream& os, util::sref<inst::type const> type)
{
    return os << type->name();
}
