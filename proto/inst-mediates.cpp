#include <algorithm>

#include "inst-mediates.h"
#include "../instance/stmt-nodes.h"
#include "../instance/scope.h"

using namespace proto;

util::sptr<inst::Statement const> DirectInst::inst(util::sref<inst::Scope>)
{
    return std::move(_stmt);
}

void DirectInst::mediate_inst(util::sref<inst::Scope>) {}

BlockMediate::BlockMediate(std::list<util::sptr<Statement const>> const& stmts, util::sref<inst::Scope> sc)
    : _stmts(stmts)
    , _mediates(NULL)
    , _inst_block(NULL)
{
    sc->add_path(util::mkref(*this));
}

util::sptr<inst::Statement const> BlockMediate::inst(util::sref<inst::Scope> sc)
{
    mediate_inst(sc);
    if (!bool(_inst_block)) {
        _inst_block.reset(new inst::Block);
        std::for_each(_mediates->begin()
                    , _mediates->end()
                    , [&](util::sptr<inst::MediateBase> const& mediate)
                      {
                          _inst_block->addStmt(std::move(mediate->inst(sc)));
                      });
    }
    return std::move(_inst_block);
}

void BlockMediate::mediate_inst(util::sref<inst::Scope> sc)
{
    if (bool(_mediates)) {
        return;
    }
    _mediates.reset(new std::list<util::sptr<inst::MediateBase>>);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement const> const& stmt)
                  {
                      _mediates->push_back(std::move(stmt->inst(sc)));
                  });
}

util::sptr<inst::Statement const> BranchMediate::inst(util::sref<inst::Scope> sc)
{
    return std::move(util::mkptr(new inst::Branch(pos
                                                , std::move(_predicate)
                                                , std::move(_consequence_mediate.inst(sc))
                                                , std::move(_alternative_mediate.inst(sc)))));
}

void BranchMediate::mediate_inst(util::sref<inst::Scope> sc)
{
    _consequence_mediate.mediate_inst(sc);
    _alternative_mediate.mediate_inst(sc);
}
