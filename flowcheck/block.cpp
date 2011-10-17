#include <algorithm>

#include <proto/node-base.h>
#include <proto/function.h>
#include <proto/variable.h>
#include <util/vector-append.h>

#include "block.h"
#include "function.h"
#include "node-base.h"
#include "filter.h"

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

void Block::compile(util::sref<proto::Block> block, util::sref<SymbolTable> st) const 
{
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement const> const& stmt)
                  {
                      block->addStmt(stmt->compile(block, st));
                  });
}

void Block::append(Block following)
{
    util::ptrs_append(_funcs, std::move(following._funcs));
    util::ptrs_append(_stmts, std::move(following._stmts));
}
