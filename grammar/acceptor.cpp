#include "acceptor.h"
#include "stmt-nodes.h"
#include "function.h"
#include "../flowcheck/node-base.h"
#include "../report/errors.h"

using namespace grammar;

void acceptor::accept_else(misc::position const& else_pos)
{
    error::else_not_match_if(else_pos);
}

void if_acceptor::accept_func(util::sptr<Function const> func)
{
    _current_branch->add_func(std::move(func));
}

void if_acceptor::accept_stmt(util::sptr<Statement const> stmt)
{
    _current_branch->add_stmt(std::move(stmt));
}

void if_acceptor::deliver_to(util::sref<acceptor> acc)
{
    if (_else_matched()) {
        acc->accept_stmt(std::move(util::mkptr(new branch(pos
                                                        , std::move(_predicate)
                                                        , std::move(_consequence)
                                                        , std::move(_alternative)))));
    } else {
        acc->accept_stmt(std::move(util::mkptr(new branch_cons_only(pos
                                                                  , std::move(_predicate)
                                                                  , std::move(_consequence)))));
    }
}

void if_acceptor::accept_else(misc::position const& else_pos)
{
    if (_else_matched()) {
        error::if_already_match_else(*_last_else_pos, else_pos);
    } else {
        _current_branch = &_alternative;
        _last_else_pos.reset(new misc::position(else_pos));
    }
}

bool if_acceptor::_else_matched() const
{
    return bool(_last_else_pos);
}

void ifnot_acceptor::accept_func(util::sptr<Function const> func)
{
    _alternative.add_func(std::move(func));
}

void ifnot_acceptor::accept_stmt(util::sptr<Statement const> stmt)
{
    _alternative.add_stmt(std::move(stmt));
}

void ifnot_acceptor::deliver_to(util::sref<acceptor> acc)
{
    acc->accept_stmt(std::move(util::mkptr(new branch_alt_only(pos
                                                             , std::move(_predicate)
                                                             , std::move(_alternative)))));
}

void Function_acceptor::accept_func(util::sptr<Function const> func)
{
    _body.add_func(std::move(func));
}

void Function_acceptor::accept_stmt(util::sptr<Statement const> stmt)
{
    _body.add_stmt(std::move(stmt));
}

void Function_acceptor::deliver_to(util::sref<acceptor> acc)
{
    acc->accept_func(std::move(util::mkptr(new Function(pos, name, param_names, std::move(_body)))));
}
