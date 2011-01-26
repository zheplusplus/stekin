#include <algorithm>

#include "inst-mediates.h"
#include "../instance/stmt-nodes.h"

using namespace proto;

util::sptr<inst::stmt_base const> direct_inst::inst(util::sref<inst::scope>)
{
    return std::move(_stmt);
}

void direct_inst::mediate_inst(util::sref<inst::scope>) {}

block_mediate::block_mediate(std::list<util::sptr<stmt_base const>> const& stmts, util::sref<inst::scope> sc)
    : _stmts(stmts)
    , _mediates(NULL)
    , _inst_block(NULL)
{
    sc->add_path(util::mkref(*this));
}

util::sptr<inst::stmt_base const> block_mediate::inst(util::sref<inst::scope> sc)
{
    if (!bool(_inst_block)) {
        mediate_inst(sc);
        _inst_block.reset(new inst::block);
        std::for_each(_mediates->begin()
                    , _mediates->end()
                    , [&](util::sptr<inst::mediate_base> const& mediate)
                      {
                          _inst_block->add_stmt(std::move(mediate->inst(sc)));
                      });
    }
    return std::move(_inst_block);
}

void block_mediate::mediate_inst(util::sref<inst::scope> sc)
{
    if (bool(_mediates)) {
        return;
    }
    _mediates.reset(new std::list<util::sptr<inst::mediate_base>>);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<stmt_base const> const& stmt)
                  {
                      _mediates->push_back(std::move(stmt->inst(sc)));
                  });
}

util::sptr<inst::stmt_base const> branch_mediate::inst(util::sref<inst::scope> sc)
{
    return std::move(util::mkptr(new inst::branch(std::move(_condition)
                                                , std::move(_valid_mediate.inst(sc))
                                                , std::move(_invalid_mediate.inst(sc)))));
}

void branch_mediate::mediate_inst(util::sref<inst::scope> sc)
{
    _valid_mediate.mediate_inst(sc);
    _invalid_mediate.mediate_inst(sc);
}

util::sptr<inst::stmt_base const> loop_mediate::inst(util::sref<inst::scope> sc)
{
    return std::move(util::mkptr(new inst::loop(std::move(_condition), std::move(_body_mediate.inst(sc)))));
}

void loop_mediate::mediate_inst(util::sref<inst::scope> sc)
{
    _body_mediate.mediate_inst(sc);
}
