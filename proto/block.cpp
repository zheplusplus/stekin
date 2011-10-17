#include <algorithm>

#include <instance/block.h>
#include <util/vector-append.h>

#include "block.h"
#include "function.h"
#include "func-inst-draft.h"
#include "variable.h"
#include "func-reference-type.h"

using namespace proto;

void Block::addStmt(util::sptr<Statement> stmt)
{
    _stmts.push_back(std::move(stmt));
}

util::sref<Function> Block::declare(misc::position const& pos
                                  , std::string const& name
                                  , std::vector<std::string> const& param_names
                                  , bool contains_void_return)
{
    util::sptr<Function> func(new Function(pos, name, param_names, contains_void_return));
    util::sref<Function> result = *func;
    _funcs.push_back(std::move(func));
    return result;
}

void Block::addTo(util::sref<FuncInstDraft> func)
{
    func->addPath(util::mkref(*this));
}

util::sptr<inst::Statement const> Block::inst(util::sref<FuncInstDraft> func, misc::trace& trace)
{
    mediateInst(func, trace);
    util::sptr<inst::Block> block(new inst::Block);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement> const& stmt)
                  {
                      block->addStmt(stmt->inst(func, trace));
                  });
    return std::move(block);
}

void Block::mediateInst(util::sref<FuncInstDraft> func, misc::trace& trace)
{
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement> const& stmt)
                  {
                      stmt->mediateInst(func, trace);
                  });
}

std::vector<util::sptr<inst::Function const>> Block::deliverFuncs()
{
    std::vector<util::sptr<inst::Function const>> result;
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement>& stmt)
                  {
                      util::ptrs_append(result, stmt->deliverFuncs());
                  });
    std::for_each(_funcs.begin()
                , _funcs.end()
                , [&](util::sptr<Function>& func)
                  {
                      util::ptrs_append(result, func->deliverFuncs());
                  });
    return std::move(result);
}
