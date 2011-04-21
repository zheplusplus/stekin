#include <algorithm>

#include "block.h"
#include "node-base.h"
#include "function.h"
#include "../flowcheck/filter.h"
#include "../flowcheck/node-base.h"
#include "../flowcheck/function.h"
#include "../proto/node-base.h"

using namespace grammar;

void Block::add_stmt(util::sptr<Statement const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

void Block::add_func(util::sptr<Function const> Function)
{
    _funcs.push_back(std::move(Function));
}

util::sptr<flchk::filter> Block::compile(util::sptr<flchk::filter> filter) const
{
    std::for_each(_funcs.begin()
                , _funcs.end()
                , [&](util::sptr<Function const> const& def)
                  {
                      def->compile(*filter);
                  });

    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement const> const& stmt)
                  {
                      stmt->compile(*filter);
                  });
    return std::move(filter);
}
