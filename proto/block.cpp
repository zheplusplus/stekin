#include <algorithm>

#include "block.h"
#include "function.h"
#include "func-inst-draft.h"
#include "variable.h"
#include "../instance/block.h"

using namespace proto;

void Block::addStmt(util::sptr<Statement> stmt)
{
    _stmts.push_back(std::move(stmt));
}

void Block::addFunc(util::sptr<Function> func)
{
    _funcs.push_back(std::move(func));
}

void Block::addTo(util::sref<FuncInstDraft> func)
{
    func->addPath(util::mkref(*this));
}

util::sptr<inst::Statement const> Block::inst(util::sref<FuncInstDraft> func
                                            , util::sref<SymbolTable> st)
{
    mediateInst(func, st);
    util::sptr<inst::Block> block(new inst::Block);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement> const& stmt)
                  {
                      block->addStmt(stmt->inst(func, st));
                  });
    return std::move(block);
}

void Block::mediateInst(util::sref<FuncInstDraft> func, util::sref<SymbolTable> st)
{
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement> const& stmt)
                  {
                      stmt->mediateInst(func, st);
                  });
}
