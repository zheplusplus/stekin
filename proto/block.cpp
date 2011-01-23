#include <algorithm>

#include "block.h"

using namespace proto;

void block::add_stmt(util::sptr<stmt_base const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

util::sptr<inst::stmt_base const> block::inst(util::sref<inst::scope const> scope) const
{
    util::sptr<inst::block> b(new inst::block);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<stmt_base const> const& stmt)
                  {
                      b->add_stmt(std::move(stmt->inst(scope)));
                  });
    return std::move(b);
}
