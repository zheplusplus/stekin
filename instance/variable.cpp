#include "variable.h"
#include "node-base.h"
#include "type.h"
#include "scope.h"
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

util::sptr<inst::Expression const> variable::call_func(misc::pos_type const& call_pos
                                                    , std::vector<util::sref<inst::type const>> const& arg_types
                                                    , std::vector<util::sptr<Expression const>> args) const
{
    return std::move(vtype->call_func(call_pos, level, stack_offset, arg_types, std::move(args)));
}

variable variable::adjust_location(int offset_diff, int lvl) const
{
    return variable(def_pos, vtype, stack_offset + offset_diff, lvl);
}
