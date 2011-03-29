#include "node-base.h"
#include "../instance/node-base.h"

using namespace proto;

util::sptr<inst::expr_base const> expr_base::inst_as_arg(util::sref<inst::scope> sc, int) const
{
    return std::move(inst(sc));
}
