#include <algorithm>

#include "inst-mediates.h"
#include "../instance/stmt-nodes.h"
#include "../instance/scope.h"

using namespace proto;

util::sptr<inst::Statement const> direct_inst::inst(util::sref<inst::scope>)
{
    return std::move(_stmt);
}

void direct_inst::mediate_inst(util::sref<inst::scope>) {}

Block_mediate::Block_mediate(std::list<util::sptr<Statement const>> const& stmts, util::sref<inst::scope> sc)
    : _stmts(stmts)
    , _mediates(NULL)
    , _inst_Block(NULL)
{
    sc->add_path(util::mkref(*this));
}

util::sptr<inst::Statement const> Block_mediate::inst(util::sref<inst::scope> sc)
{
    mediate_inst(sc);
    if (!bool(_inst_Block)) {
        _inst_Block.reset(new inst::Block);
        std::for_each(_mediates->begin()
                    , _mediates->end()
                    , [&](util::sptr<inst::mediate_base> const& mediate)
                      {
                          _inst_Block->add_stmt(std::move(mediate->inst(sc)));
                      });
    }
    return std::move(_inst_Block);
}

void Block_mediate::mediate_inst(util::sref<inst::scope> sc)
{
    if (bool(_mediates)) {
        return;
    }
    _mediates.reset(new std::list<util::sptr<inst::mediate_base>>);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement const> const& stmt)
                  {
                      _mediates->push_back(std::move(stmt->inst(sc)));
                  });
}

util::sptr<inst::Statement const> branch_mediate::inst(util::sref<inst::scope> sc)
{
    return std::move(util::mkptr(new inst::branch(pos
                                                , std::move(_predicate)
                                                , std::move(_consequence_mediate.inst(sc))
                                                , std::move(_alternative_mediate.inst(sc)))));
}

void branch_mediate::mediate_inst(util::sref<inst::scope> sc)
{
    _consequence_mediate.mediate_inst(sc);
    _alternative_mediate.mediate_inst(sc);
}
