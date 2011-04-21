#include <algorithm>

#include "block.h"
#include "inst-mediates.h"
#include "../instance/node-base.h"
#include "../instance/block.h"

using namespace proto;

void block::add_stmt(util::sptr<Statement const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

util::sptr<inst::mediate_base> block::inst(util::sref<inst::scope> scope) const
{
    return std::move(util::mkmptr(new block_mediate(_stmts, scope)));
}

std::list<util::sptr<Statement const>> const& block::get_stmts() const
{
    return _stmts;
}
