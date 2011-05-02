#include "stmt-nodes.h"
#include "function.h"
#include "../flowcheck/node-base.h"
#include "../flowcheck/symbol-def-filter.h"
#include "../flowcheck/function.h"
#include "../proto/node-base.h"

using namespace grammar;

static util::sptr<flchk::Filter> mkSymDefFilter(util::sref<flchk::Filter> ext_filter)
{
    return std::move(util::mkmptr(new flchk::SymbolDefFilter(ext_filter->getSymbols())));
}

void Arithmetics::compile(util::sref<flchk::Filter> filter) const
{
    filter->addArith(pos, std::move(expr->compile(filter)));
}

void Branch::compile(util::sref<flchk::Filter> filter) const
{
    filter->addBranch(pos
                    , std::move(predicate->compile(filter))
                    , std::move(consequence.compile(std::move(mkSymDefFilter(filter))))
                    , std::move(alternative.compile(std::move(mkSymDefFilter(filter)))));
}

void BranchConsqOnly::compile(util::sref<flchk::Filter> filter) const
{
    filter->addBranch(pos
                    , std::move(predicate->compile(filter))
                    , std::move(consequence.compile(std::move(mkSymDefFilter(filter)))));
}

void BranchAlterOnly::compile(util::sref<flchk::Filter> filter) const
{
    filter->addBranchAlterOnly(pos
                             , std::move(predicate->compile(filter))
                             , std::move(alternative.compile(std::move(mkSymDefFilter(filter)))));
}

void Return::compile(util::sref<flchk::Filter> filter) const
{
    filter->addReturn(pos, std::move(ret_val->compile(filter)));
}

void ReturnNothing::compile(util::sref<flchk::Filter> filter) const
{
    filter->addReturnNothing(pos);
}

void VarDef::compile(util::sref<flchk::Filter> filter) const
{
    filter->defVar(pos, name, std::move(init->compile(filter)));
}
