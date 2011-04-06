#include "common.h"

using namespace test;

std::ostream& test::operator<<(std::ostream& os, misc::pos_type const& pos)
{
    return os << pos.str();
}
