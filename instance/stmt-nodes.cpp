#include <algorithm>

#include "stmt-nodes.h"
#include "operation.h"
#include "type.h"
#include "../output/statement-writer.h"

using namespace inst;

Branch::Branch(misc::position const& pos
             , util::sptr<Expression const> p
             , util::sptr<Statement const> c
             , util::sptr<Statement const> a)
    : predicate(std::move(p))
    , consequence(std::move(c))
    , alternative(std::move(a))
{
    predicate->typeof()->checkCondType(pos);
}

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
    output::refThisLevel(offset, init->typeof()->exportedName());
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
