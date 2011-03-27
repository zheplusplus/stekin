#include "variable.h"
#include "type.h"

using namespace inst;

bool variable::operator<(variable const& rhs) const
{
    if (vtype != rhs.vtype) {
        return vtype < rhs.vtype;
    }
    if (stack_offset != rhs.stack_offset) {
        return stack_offset < rhs.stack_offset;
    }
    return level < rhs.level;
}

bool variable::operator==(variable const& rhs) const
{
    return vtype == rhs.vtype && stack_offset == rhs.stack_offset && level == rhs.level;
}

bool variable::operator!=(variable const& rhs) const
{
    return !operator==(rhs);
}
