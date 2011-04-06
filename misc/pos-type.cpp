#include <sstream>

#include "pos-type.h"

using namespace misc;

std::string pos_type::str() const
{
    std::stringstream ss;
    ss << "Line: " << line;
    return ss.str();
}

bool pos_type::operator==(pos_type const& rhs) const
{
    return line == rhs.line;
}
