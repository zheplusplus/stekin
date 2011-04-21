#include <algorithm>

#include "block.h"
#include "inst-mediates.h"
#include "../instance/node-base.h"
#include "../instance/block.h"

using namespace proto;

void Block::addStmt(util::sptr<Statement const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

util::sptr<inst::mediate_base> Block::inst(util::sref<inst::scope> scope) const
{
    return std::move(util::mkmptr(new BlockMediate(_stmts, scope)));
}

std::list<util::sptr<Statement const>> const& Block::get_stmts() const
{
    return _stmts;
}
