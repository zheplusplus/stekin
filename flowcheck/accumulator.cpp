#include "accumulator.h"
#include "stmt-nodes.h"
#include "function.h"
#include "../proto/node-base.h"
#include "../report/errors.h"
#include "../report/warnings.h"

using namespace flchk;

void accumulator::add_func_ret(misc::position const& pos, util::sptr<Expression const> ret_val)
{
    _check_not_terminated(pos);
    _set_terminated_not_by_void_return(pos);
    _Block.add_stmt(std::move(util::mkptr(new func_ret(pos, std::move(ret_val)))));
}

void accumulator::add_func_ret_nothing(misc::position const& pos)
{
    _check_not_terminated(pos);
    _set_terminated_by_void_return(pos);
    _Block.add_stmt(std::move(util::mkptr(new func_ret_nothing(pos))));
}

void accumulator::add_arith(misc::position const& pos, util::sptr<Expression const> expr)
{
    _check_not_terminated(pos);
    _Block.add_stmt(std::move(util::mkptr(new arithmetics(pos, std::move(expr)))));
}

void accumulator::add_branch(misc::position const& pos
                           , util::sptr<Expression const> predicate
                           , accumulator consequence
                           , accumulator alternative)
{
    _check_not_terminated(pos);
    _check_branches_temination(consequence, alternative);
    _set_termination_by_sub_accumulator(consequence);
    _set_termination_by_sub_accumulator(alternative);
    _Block.add_stmt(std::move(util::mkptr(new branch(pos
                                                   , std::move(predicate)
                                                   , std::move(consequence._Block)
                                                   , std::move(alternative._Block)))));
}

void accumulator::add_branch(misc::position const& pos
                           , util::sptr<Expression const> predicate
                           , accumulator consequence)
{
    _check_not_terminated(pos);
    _set_termination_by_sub_accumulator(consequence);
    _Block.add_stmt(std::move(util::mkptr(new branch(pos
                                                   , std::move(predicate)
                                                   , std::move(consequence._Block)
                                                   , std::move(Block())))));
}

void accumulator::add_branch_alt_only(misc::position const& pos
                                    , util::sptr<Expression const> predicate
                                    , accumulator alternative)
{
    _check_not_terminated(pos);
    _set_termination_by_sub_accumulator(alternative);
    _Block.add_stmt(std::move(util::mkptr(new branch(pos
                                                   , std::move(predicate)
                                                   , std::move(Block())
                                                   , std::move(alternative._Block)))));
}

void accumulator::add_Block(accumulator b)
{
    _Block.append(std::move(b._Block));
    _set_self_terminated(std::move(b));
}

void accumulator::def_var(misc::position const& pos, std::string const& name, util::sptr<Expression const> init)
{
    _check_not_terminated(pos);
    _Block.add_stmt(std::move(util::mkptr(new var_def(pos, name, std::move(init)))));
}

void accumulator::def_func(misc::position const& pos
                         , std::string const& name
                         , std::vector<std::string> const& param_names
                         , accumulator body)
{
    _Block.def_func(pos
                  , name
                  , param_names
                  , std::move(body._Block)
                  , body._contains_void_return || !body._terminated());
}

Block accumulator::deliver()
{
    return std::move(_Block);
}

void accumulator::_set_terminated_by_void_return(misc::position const& pos)
{
    _set_terminated_not_by_void_return(pos);
    _contains_void_return = true;
}

void accumulator::_set_terminated_not_by_void_return(misc::position const& pos)
{
    _termination_pos.reset(new misc::position(pos));
}

void accumulator::_set_termination_by_sub_accumulator(accumulator const& sub)
{
    _contains_void_return = _contains_void_return || sub._contains_void_return;
}

void accumulator::_check_branches_temination(accumulator const& consequence, accumulator const& alternative)
{
    if (consequence._terminated() || alternative._terminated()) {
        warning::one_or_two_branches_terminated(*consequence._termination_pos, *alternative._termination_pos);
    }
}

void accumulator::_check_not_terminated(misc::position const& pos)
{
    if (_terminated()) {
        _report_terminated(pos);
    }
}

void accumulator::_report_terminated(misc::position const& pos)
{
    if (!_error_reported) {
        error::flow_terminated(pos, _termination_pos.cp());
        _error_reported = true;
    }
}

bool accumulator::_terminated() const
{
    return bool(_termination_pos);
}

void accumulator::_set_self_terminated(accumulator term)
{
    _set_termination_by_sub_accumulator(term);
    _termination_pos = std::move(term._termination_pos);
    if (bool(_termination_pos)) {
        _error_reported = true;
    }
}
