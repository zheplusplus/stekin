#include <algorithm>

#include "block.h"
#include "function.h"
#include "node-base.h"
#include "../proto/node-base.h"
#include "../proto/scope.h"
#include "../proto/function.h"

using namespace flchk;

void Block::addStmt(util::sptr<Statement const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

void Block::defFunc(misc::position const& pos
                  , std::string const& name
                  , std::vector<std::string> const& param_names
                  , Block body
                  , bool contains_void_return)
{
    _funcs.push_back(std::move(util::mkptr(new Function(pos
                                                      , name
                                                      , param_names
                                                      , std::move(body)
                                                      , contains_void_return))));
}

void Block::compile(util::sref<proto::Scope> scope) const 
{
    std::vector<util::sref<proto::Function>> decls;
    decls.reserve(_funcs.size());
    std::for_each(_funcs.begin()
                , _funcs.end()
                , [&](util::sptr<Function const> const& func)
                  {
                      decls.push_back(func->declare(scope));
                  });

    std::vector<util::sref<proto::Function>>::iterator func_decl_iter = decls.begin();
    std::for_each(_funcs.begin()
                , _funcs.end()
                , [&](util::sptr<Function const> const& func)
                  {
                      func->body.compile(*func_decl_iter++);
                  });

    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement const> const& stmt)
                  {
                      scope->addStmt(stmt->compile(scope));
                  });
}

void Block::append(Block following)
{
    std::for_each(following._funcs.begin()
                , following._funcs.end()
                , [&](util::sptr<Function const>& func)
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
