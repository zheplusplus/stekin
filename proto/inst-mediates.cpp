#include <algorithm>

#include "inst-mediates.h"
#include "function.h"
#include "../instance/stmt-nodes.h"
#include "../instance/scope.h"

using namespace proto;

void DirectInst::addTo(util::sref<inst::Scope>) {}

util::sptr<inst::Statement const> DirectInst::inst(util::sref<inst::Scope>)
{
    return std::move(_stmt);
}

void DirectInst::mediateInst(util::sref<inst::Scope>) {}

void BlockMediate::addTo(util::sref<inst::Scope> scope)
{
    scope->addPath(util::mkref(*this));
}

util::sptr<inst::Statement const> BlockMediate::inst(util::sref<inst::Scope> scope)
{
    mediateInst(scope);
    util::sptr<inst::Block> block(new inst::Block);
    std::for_each(_mediates->begin()
                , _mediates->end()
                , [&](util::sptr<inst::MediateBase> const& mediate)
                  {
                      block->addStmt(mediate->inst(scope));
                  });
    return std::move(block);
}

void BlockMediate::mediateInst(util::sref<inst::Scope> scope)
{
    if (bool(_mediates)) {
        return;
    }
    _mediates.reset(new std::list<util::sptr<inst::MediateBase>>);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement const> const& stmt)
                  {
                      _mediates->push_back(stmt->inst(scope));
                      _mediates->back()->addTo(scope);
                  });
}

void BranchMediate::addTo(util::sref<inst::Scope> scope)
{
    _consequence_mediate->addTo(scope);
    _alternative_mediate->addTo(scope);
}

util::sptr<inst::Statement const> BranchMediate::inst(util::sref<inst::Scope> scope)
{
    return util::mkptr(new inst::Branch(pos
                                      , std::move(_predicate)
                                      , _consequence_mediate->inst(scope)
                                      , _alternative_mediate->inst(scope)));
}

void BranchMediate::mediateInst(util::sref<inst::Scope> scope)
{
    _consequence_mediate->mediateInst(scope);
    _alternative_mediate->mediateInst(scope);
}
