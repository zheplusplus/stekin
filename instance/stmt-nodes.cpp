#include <algorithm>

#include <output/stmt-writer.h>
#include <output/expr-writer.h>

#include "stmt-nodes.h"

using namespace inst;

void Arithmetics::write() const
{
    expr->writePipeDef(level);
    expr->write();
    output::endOfStatement();
}

void Branch::write() const
{
    predicate->writePipeDef(level);
    output::branchIf();
    output::beginExpr();
    predicate->write();
    output::endExpr();
    consequence->write();
    output::branchElse();
    alternative->write();
}

void Initialization::write() const
{
    init->writePipeDef(level);
    output::initThisLevel(offset, type->exportedName());
    output::beginExpr();
    init->write();
    output::endExpr();
    output::endOfStatement();
    type->writeResEntry(offset);
}

void Return::write() const
{
    ret_val->writePipeDef(level);
    output::kwReturn();
    ret_val->write();
    output::endOfStatement();
}

void ReturnNothing::write() const
{
    output::returnNothing();
}
