#include "variable.h"
#include "node-base.h"
#include "type.h"
#include "../output/statement-writer.h"

using namespace inst;

bool Variable::operator<(Variable const& rhs) const
{
    if (type != rhs.type) {
        return type < rhs.type;
    }
    if (stack_offset != rhs.stack_offset) {
        return stack_offset < rhs.stack_offset;
    }
    return level < rhs.level;
}

bool Variable::operator==(Variable const& rhs) const
{
    return type == rhs.type && stack_offset == rhs.stack_offset && level == rhs.level;
}

bool Variable::operator!=(Variable const& rhs) const
{
    return !operator==(rhs);
}

util::sptr<inst::Expression const> Variable::call(
                                        misc::position const& call_pos
                                      , std::vector<util::sref<inst::Type const>> const& arg_types
                                      , std::vector<util::sptr<Expression const>> args) const
{
    return std::move(type->call(call_pos, level, stack_offset, arg_types, std::move(args)));
}

Variable Variable::adjustLocation(int offset_diff, int lvl) const
{
    return Variable(def_pos, type, stack_offset + offset_diff, lvl);
}
