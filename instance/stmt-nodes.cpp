#include <algorithm>

#include "stmt-nodes.h"
#include "../output/statement-writer.h"

using namespace inst;

void Arithmetics::write() const
{
    expr->write();
    output::endOfStatement();
}

void Branch::write() const
{
    output::branchIf();
    predicate->write();
    consequence->write();
    output::branchElse();
    alternative->write();
}

void Initialization::write() const
{
    output::refThisLevel(offset, type_exported_name);
    output::signAssign();
    init->write();
    output::endOfStatement();
}

void Return::write() const
{
    output::kwReturn();
    ret_val->write();
    output::endOfStatement();
}

void ReturnNothing::write() const
{
    output::returnNothing();
}
