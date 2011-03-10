#include <algorithm>

#include "block.h"
#include "node-base.h"
#include "function.h"

using namespace grammar;

void block::add_stmt(util::sptr<stmt_base const> stmt)
{
    _flow.push_back(std::move(stmt));
}

void block::add_func(util::sptr<struct function const> function)
{
    _functions.push_back(std::move(function));
}

void block::compile(util::sref<proto::scope> scope) const 
{
    std::vector<util::sref<proto::function>> func_decls;
    func_decls.reserve(_functions.size());
    std::for_each(_functions.begin()
                , _functions.end()
                , [&](util::sptr<function const> const& def)
                  {
                      func_decls.push_back(def->declare(scope));
                  });

    std::vector<util::sref<proto::function>>::iterator func_decl = func_decls.begin();
    std::for_each(_functions.begin()
                , _functions.end()
                , [&](util::sptr<function const> const& def)
                  {
                      def->compile(*func_decl++);
                  });

    std::for_each(_flow.begin()
                , _flow.end()
                , [&](util::sptr<stmt_base const> const& stmt)
                  {
                      stmt->compile(scope);
                  });
}
