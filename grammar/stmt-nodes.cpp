#include "stmt-nodes.h"
#include "function.h"
#include "../flowcheck/node-base.h"
#include "../flowcheck/filter.h"
#include "../flowcheck/function.h"
#include "../proto/node-base.h"

using namespace grammar;

void arithmetics::compile(util::sref<flchk::filter> filter)
{
    filter->add_arith(pos, std::move(_expr));
}

void branch::compile(util::sref<flchk::filter> filter)
{
    filter->add_branch(pos
                     , std::move(_predicate)
                     , std::move(_consequence.compile(std::move(util::mkmptr(new flchk::symbol_def_filter))))
                     , std::move(_alternative.compile(std::move(util::mkmptr(new flchk::symbol_def_filter)))));
}

void branch_cons_only::compile(util::sref<flchk::filter> filter)
{
    filter->add_branch(pos
                     , std::move(_predicate)
                     , std::move(_consequence.compile(std::move(util::mkmptr(new flchk::symbol_def_filter)))));
}

void branch_alt_only::compile(util::sref<flchk::filter> filter)
{
    filter->add_branch_alt_only(
                       pos
                     , std::move(_predicate)
                     , std::move(_alternative.compile(std::move(util::mkmptr(new flchk::symbol_def_filter)))));
}

void func_ret::compile(util::sref<flchk::filter> filter)
{
    filter->add_func_ret(pos, std::move(_ret_val));
}

void func_ret_nothing::compile(util::sref<flchk::filter> filter)
{
    filter->add_func_ret_nothing(pos);
}

void var_def::compile(util::sref<flchk::filter> filter)
{
    filter->def_var(pos, _name, std::move(_init));
}
