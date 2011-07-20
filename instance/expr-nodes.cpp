#include <algorithm>

#include "expr-nodes.h"
#include "../output/func-writer.h"
#include "../output/statement-writer.h"

using namespace inst;

void IntLiteral::write() const
{
    output::writeInt(value);
}

void FloatLiteral::write() const
{
    output::writeFloat(value);
}

void BoolLiteral::write() const
{
    output::writeBool(value);
}

void Reference::write() const
{
    output::refLevel(address.offset, address.level, type->exportedName());
}

void Call::write() const
{
    output::writeCallBegin(call_sn);
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& expr)
                  {
                       output::writeArgSeparator();
                       expr->write();
                  });
    output::writeCallEnd();
}

void FuncReference::write() const
{
    output::writeFuncReference(size);
    std::for_each(args.begin()
                , args.end()
                , [&](ArgInfo const& arg)
                  {
                      output::funcReferenceNextVariable(arg.self_offset
                                                      , output::StackVarRec(arg.type->exportedName()
                                                                          , arg.address.offset
                                                                          , arg.address.level));
                  });
}

void BinaryOp::write() const
{
    output::beginExpr();
    lhs->write();
    output::writeOperator(op);
    rhs->write();
    output::endExpr();
}

void PreUnaryOp::write() const
{
    output::beginExpr();
    output::writeOperator(op);
    rhs->write();
    output::endExpr();
}

void Conjunction::write() const
{
    output::beginExpr();
    lhs->write();
    output::writeOperator("&&");
    rhs->write();
    output::endExpr();
}

void Disjunction::write() const
{
    output::beginExpr();
    lhs->write();
    output::writeOperator("||");
    rhs->write();
    output::endExpr();
}

void Negation::write() const
{
    output::beginExpr();
    output::writeOperator("!");
    rhs->write();
    output::endExpr();
}
