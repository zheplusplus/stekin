#include "filter.h"
#include "node-base.h"
#include "function.h"
#include "../proto/node-base.h"
#include "../report/errors.h"

using namespace flchk;

void filter::add_func_ret(misc::pos_type const& pos, util::sptr<expr_base const> ret_val)
{
    _accumulator.add_func_ret(pos, std::move(ret_val));
}

void filter::add_func_ret_nothing(misc::pos_type const& pos)
{
    _accumulator.add_func_ret_nothing(pos);
}

void filter::add_arith(misc::pos_type const& pos, util::sptr<expr_base const> expr)
{
    _accumulator.add_arith(pos, std::move(expr));
}

void filter::add_branch(misc::pos_type const& pos
                      , util::sptr<expr_base const> predicate
                      , util::sptr<filter> consequence
                      , util::sptr<filter> alternative)
{
    _accumulator.add_branch(pos
                          , std::move(predicate)
                          , std::move(consequence->_accumulator)
                          , std::move(alternative->_accumulator));
}

void filter::add_branch(misc::pos_type const& pos
                      , util::sptr<expr_base const> predicate
                      , util::sptr<filter> consequence)
{
    _accumulator.add_branch(pos, std::move(predicate), std::move(consequence->_accumulator));
}

void filter::add_branch_alt_only(misc::pos_type const& pos
                               , util::sptr<expr_base const> predicate
                               , util::sptr<filter> alternative)
{
    _accumulator.add_branch_alt_only(pos, std::move(predicate), std::move(alternative->_accumulator));
}

void filter::def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const> init)
{
    _accumulator.def_var(pos, name, std::move(init));
}

void filter::def_func(misc::pos_type const& pos
                    , std::string const& name
                    , std::vector<std::string> const& param_names
                    , util::sptr<filter> body)
{
    _accumulator.def_func(pos, name, param_names, std::move(body->_accumulator));
}

block filter::deliver()
{
    return std::move(_accumulator.deliver());
}

void symbol_def_filter::def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const>)
{
    error::forbid_def_var(pos, name);
}

void symbol_def_filter::def_func(misc::pos_type const& pos
                               , std::string const& name
                               , std::vector<std::string> const&
                               , util::sptr<filter>)
{
    error::forbid_def_func(pos, name);
}
