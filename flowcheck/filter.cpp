#include "filter.h"
#include "node-base.h"
#include "function.h"
#include "../proto/node-base.h"
#include "../report/errors.h"

using namespace flchk;

void filter::add_func_ret(misc::position const& pos, util::sptr<Expression const> ret_val)
{
    _accumulator.add_func_ret(pos, std::move(ret_val->fold()));
}

void filter::add_func_ret_nothing(misc::position const& pos)
{
    _accumulator.add_func_ret_nothing(pos);
}

void filter::addArith(misc::position const& pos, util::sptr<Expression const> expr)
{
    _accumulator.addArith(pos, std::move(expr->fold()));
}

void filter::add_branch(misc::position const& pos
                      , util::sptr<Expression const> predicate
                      , util::sptr<filter> consequence
                      , util::sptr<filter> alternative)
{
    util::sptr<Expression const> pred(std::move(predicate->fold()));
    if (pred->is_literal()) {
        _accumulator.add_block(std::move((pred->bool_value() ? consequence : alternative)->_accumulator));
        return;
    }
    _accumulator.add_branch(pos
                          , std::move(pred)
                          , std::move(consequence->_accumulator)
                          , std::move(alternative->_accumulator));
}

void filter::add_branch(misc::position const& pos
                      , util::sptr<Expression const> predicate
                      , util::sptr<filter> consequence)
{
    util::sptr<Expression const> pred(std::move(predicate->fold()));
    if (pred->is_literal()) {
        if (pred->bool_value()) {
            _accumulator.add_block(std::move(consequence->_accumulator));
        }
        return;
    }
    _accumulator.add_branch(pos, std::move(pred), std::move(consequence->_accumulator));
}

void filter::add_branch_alt_only(misc::position const& pos
                               , util::sptr<Expression const> predicate
                               , util::sptr<filter> alternative)
{
    util::sptr<Expression const> pred(std::move(predicate->fold()));
    if (pred->is_literal()) {
        if (!pred->bool_value()) {
            _accumulator.add_block(std::move(alternative->_accumulator));
        }
        return;
    }
    _accumulator.add_branch_alt_only(pos, std::move(pred), std::move(alternative->_accumulator));
}

void filter::def_var(misc::position const& pos, std::string const& name, util::sptr<Expression const> init)
{
    _accumulator.def_var(pos, name, std::move(init->fold()));
}

void filter::def_func(misc::position const& pos
                    , std::string const& name
                    , std::vector<std::string> const& param_names
                    , util::sptr<filter> body)
{
    _accumulator.def_func(pos, name, param_names, std::move(body->_accumulator));
}

Block filter::deliver()
{
    return std::move(_accumulator.deliver());
}

void symbol_def_filter::def_var(misc::position const& pos, std::string const& name, util::sptr<Expression const>)
{
    error::forbid_def_var(pos, name);
}

void symbol_def_filter::def_func(misc::position const& pos
                               , std::string const& name
                               , std::vector<std::string> const&
                               , util::sptr<filter>)
{
    error::forbid_def_func(pos, name);
}
