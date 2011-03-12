#include <algorithm>

#include "block.h"
#include "node-base.h"
#include "function.h"
#include "../flowcheck/filter.h"
#include "../flowcheck/node-base.h"
#include "../flowcheck/function.h"
#include "../proto/node-base.h"

using namespace grammar;

void block::add_stmt(util::sptr<stmt_base> stmt)
{
    _stmts.push_back(std::move(stmt));
}

void block::add_func(util::sptr<function const> function)
{
    _funcs.push_back(std::move(function));
}

util::sptr<flchk::filter> block::compile(util::sptr<flchk::filter> filter) const
{
    std::for_each(_funcs.begin()
                , _funcs.end()
                , [&](util::sptr<function const> const& def)
                  {
                      def->compile(*filter);
                  });

    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<stmt_base> const& stmt)
                  {
                      stmt->compile(*filter);
                  });
    return std::move(filter);
}
