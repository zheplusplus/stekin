#include <algorithm>

#include "block.h"
#include "inst-mediates.h"

using namespace proto;

void block::add_stmt(util::sptr<stmt_base const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

util::sptr<inst::mediate_base> block::inst(util::sref<inst::scope> scope) const
{
    return std::move(util::mkmptr(new block_mediate(_stmts, scope)));
}

std::list<util::sptr<stmt_base const>> const& block::get_stmts() const
{
    return _stmts;
}
