#include "stmt-nodes.h"
#include "function.h"
#include "../flowcheck/node-base.h"
#include "../flowcheck/filter.h"
#include "../flowcheck/function.h"
#include "../proto/node-base.h"

using namespace grammar;

void Arithmetics::compile(util::sref<flchk::Filter> filter) const
{
    filter->addArith(pos, std::move(expr->compile()));
}

void Branch::compile(util::sref<flchk::Filter> filter) const
{
    filter->addBranch(
               pos
             , std::move(predicate->compile())
             , std::move(consequence.compile(std::move(util::mkmptr(new flchk::SymbolDefFilter))))
             , std::move(alternative.compile(std::move(util::mkmptr(new flchk::SymbolDefFilter)))));
}

void BranchConsqOnly::compile(util::sref<flchk::Filter> filter) const
{
    filter->addBranch(
               pos
             , std::move(predicate->compile())
             , std::move(consequence.compile(std::move(util::mkmptr(new flchk::SymbolDefFilter)))));
}

void BranchAlterOnly::compile(util::sref<flchk::Filter> filter) const
{
    filter->addBranchAlterOnly(
               pos
             , std::move(predicate->compile())
             , std::move(alternative.compile(std::move(util::mkmptr(new flchk::SymbolDefFilter)))));
}

void Return::compile(util::sref<flchk::Filter> filter) const
{
    filter->addReturn(pos, std::move(ret_val->compile()));
}

void ReturnNothing::compile(util::sref<flchk::Filter> filter) const
{
    filter->addReturnNothing(pos);
}

void VarDef::compile(util::sref<flchk::Filter> filter) const
{
    filter->defVar(pos, name, std::move(init->compile()));
}
