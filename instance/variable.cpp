#include "variable.h"
#include "type.h"
#include "../output/statement-writer.h"

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

void static_level::write(int offset, std::string const& name) const
{
    output::ref_level(offset, level, name);
}

util::sptr<variable_level const> static_level::copy() const
{
    return std::move(util::mkptr(new static_level(level)));
}

void current_level::write(int offset, std::string const& name) const
{
    output::ref_this_level(offset, name);
}

util::sptr<variable_level const> current_level::copy() const
{
    return std::move(util::mkptr(new current_level));
}
