#include <algorithm>

#include "block.h"
#include "inst-mediates.h"
#include "function.h"
#include "../instance/node-base.h"
#include "../instance/block.h"

using namespace proto;

void Block::addStmt(util::sptr<Statement const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

void Block::addFunc(util::sptr<Function> func)
{
    _funcs.push_back(std::move(func));
}

util::sptr<inst::MediateBase> Block::inst() const
{
    return util::mkptr(new BlockMediate(_stmts));
}
