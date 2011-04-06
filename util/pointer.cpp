#include <sstream>

#include "pointer.h"

using namespace util;

std::string id::str() const
{
    std::stringstream ss;
    ss << _id;
    return ss.str();
}

bool id::operator<(id const& rhs) const
{
    return _id < rhs._id;
}

bool id::operator==(id const& rhs) const
{
    return _id == rhs._id;
}

bool id::operator!=(id const& rhs) const
{
    return !operator==(rhs);
}
