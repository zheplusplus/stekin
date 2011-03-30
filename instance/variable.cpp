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

util::sptr<inst::expr_base const> variable::call_func(util::sref<inst::scope> scope
                                                    , std::vector<util::sref<inst::type const>> const& arg_types
                                                    , std::vector<util::sptr<expr_base const>> args) const
{
    return std::move(vtype->call_func(scope->level(), stack_offset, arg_types, std::move(args)));
}

variable variable::adjust_offset(int offset) const
{
    return variable(def_pos, vtype, stack_offset + offset, level);
}
