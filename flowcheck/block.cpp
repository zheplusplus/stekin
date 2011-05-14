#include <algorithm>

#include "block.h"
#include "function.h"
#include "node-base.h"
#include "filter.h"
#include "../proto/node-base.h"
#include "../proto/function.h"
#include "../proto/variable.h"
#include "../proto/func-reference-type.h"

using namespace flchk;

void Block::addStmt(util::sptr<Statement const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

util::sref<Function> Block::defFunc(misc::position const& pos
                                  , std::string const& name
                                  , std::vector<std::string> const& param_names
                                  , util::sptr<Filter> body)
{
    _funcs.push_back(util::mkptr(new Function(pos, name, param_names, std::move(body))));
    return *_funcs.back();
}

void Block::compile(util::sref<proto::Scope> scope, util::sref<SymbolTable> st) const 
{
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement const> const& stmt)
                  {
                      scope->addStmt(stmt->compile(scope, st));
                  });
}

void Block::append(Block following)
{
    std::for_each(following._funcs.begin()
                , following._funcs.end()
                , [&](util::sptr<Function>& func)
                  {
                      _funcs.push_back(std::move(func));
                  });

    std::for_each(following._stmts.begin()
                , following._stmts.end()
                , [&](util::sptr<Statement const>& stmt)
                  {
                      _stmts.push_back(std::move(stmt));
                  });
}
