#include <algorithm>

#include <output/func-writer.h>
#include <output/expr-writer.h>

#include "expr-nodes.h"

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

void StringLiteral::write() const
{
    output::writeString(value.c_str(), value.size());
}

void EmptyListLiteral::write() const
{
    output::emptyList();
}

void ListLiteral::write() const
{
    output::listBegin(value.size(), member_type->exportedName());
    std::for_each(value.begin()
                , value.end()
                , [&](util::sptr<Expression const> const& member)
                  {
                      output::listNextMember();
                      member->write();
                  });
    output::listEnd();
}

void ListElement::write() const
{
    output::pipeElement();
}

void ListIndex::write() const
{
    output::pipeIndex();
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

void MemberCall::write() const
{
    object->write();
    output::memberCallBegin(name);
    if (args.empty()) {
        output::memberCallEnd();
        return;
    }
    (*args.begin())->write();
    std::for_each(++args.begin()
                , args.end()
                , [&](util::sptr<Expression const> const& arg)
                  {
                      output::writeArgSeparator();
                      arg->write();
                  });
    output::memberCallEnd();
}

void FuncReference::write() const
{
    output::writeFuncReference(size);
    std::for_each(args.begin()
                , args.end()
                , [&](ArgInfo const& arg)
                  {
                      output::funcReferenceNextVariable(arg.self_offset
                                                      , arg.type->makeParameter(arg.address));
                  });
}

void ListAppend::write() const
{
    output::listAppendBegin();
    lhs->write();
    output::writeArgSeparator();
    rhs->write();
    output::listAppendEnd();
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

static void writePipeDefInList(std::vector<util::sptr<Expression const>> const& list, int level)
{
    std::for_each(list.begin()
                , list.end()
                , [&](util::sptr<Expression const> const& element)
                  {
                      element->writePipeDef(level);
                  });
}

void ListLiteral::writePipeDef(int level) const
{
    writePipeDefInList(value, level);
}

void Call::writePipeDef(int level) const
{
    writePipeDefInList(args, level);
}

void MemberCall::writePipeDef(int level) const
{
    object->writePipeDef(level);
    writePipeDefInList(args, level);
}

void ListAppend::writePipeDef(int level) const
{
    lhs->writePipeDef(level);
    rhs->writePipeDef(level);
}

void BinaryOp::writePipeDef(int level) const
{
    lhs->writePipeDef(level);
    rhs->writePipeDef(level);
}

void PreUnaryOp::writePipeDef(int level) const
{
    rhs->writePipeDef(level);
}

void Conjunction::writePipeDef(int level) const
{
    lhs->writePipeDef(level);
    rhs->writePipeDef(level);
}

void Disjunction::writePipeDef(int level) const
{
    lhs->writePipeDef(level);
    rhs->writePipeDef(level);
}

void Negation::writePipeDef(int level) const
{
    rhs->writePipeDef(level);
}
