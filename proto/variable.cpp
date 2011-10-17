#include "variable.h"
#include "func-inst-draft.h"
#include "type.h"

using namespace proto;

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

util::sref<proto::FuncInstDraft> Variable::call(std::vector<util::sref<Type const>> const& arg_types
                                              , misc::trace& trace) const
{
    return type->call(level, stack_offset, arg_types, trace);
}

Variable Variable::adjustLocation(int offset_diff, int lvl) const
{
    return Variable(def_pos, type, stack_offset + offset_diff, lvl);
}
