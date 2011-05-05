#include <algorithm>

#include "block.h"
#include "function.h"
#include "../instance/node-base.h"
#include "../instance/block.h"
#include "../instance/scope.h"

using namespace proto;

void Block::addMediate(util::sptr<inst::MediateBase> mediate)
{
    _mediates.push_back(std::move(mediate));
}

void Block::addFunc(util::sptr<Function> func)
{
    _funcs.push_back(std::move(func));
}

void Block::addTo(util::sref<inst::Scope> scope)
{
    scope->addPath(util::mkref(*this));
}

util::sptr<inst::Statement const> Block::inst(util::sref<inst::Scope> scope)
{
    mediateInst(scope);
    util::sptr<inst::Block> block(new inst::Block);
    std::for_each(_mediates.begin()
                , _mediates.end()
                , [&](util::sptr<inst::MediateBase> const& mediate)
                  {
                      block->addStmt(mediate->inst(scope));
                  });
    return std::move(block);
}

void Block::mediateInst(util::sref<inst::Scope> scope)
{
    std::for_each(_mediates.begin()
                , _mediates.end()
                , [&](util::sptr<inst::MediateBase> const& mediate)
                  {
                      mediate->mediateInst(scope);
                  });
}
