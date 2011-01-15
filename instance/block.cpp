#include <algorithm>

#include "block.h"

using namespace inst;

void block::add_stmt(util::sptr<stmt_base const> stmt)
{
    _stmts.push_back(std::move(stmt));
}
