#include <algorithm>

#include <instance/block.h>
#include <instance/expr-nodes.h>
#include <instance/list-pipe.h>
#include <instance/stmt-nodes.h>
#include <instance/function.h>
#include <instance/built-in.h>
#include <instance/types.h>
#include <util/string.h>

#include "test-common.h"

using namespace test;
using namespace inst;

void Block::addStmt(util::sptr<Statement const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

void Block::write() const
{
    DataTree::actualOne()(BLOCK_BEGIN);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement const> const& stmt)
                  {
                      stmt->write();
                  });
    DataTree::actualOne()(BLOCK_END);
}

void Arithmetics::write() const
{
    DataTree::actualOne()(ARITHMETICS);
}

void Branch::write() const
{
    DataTree::actualOne()(BRANCH);
    predicate->write();
    consequence->write();
    alternative->write();
}

void Initialization::write() const
{
    DataTree::actualOne()(INITIALIZATION, util::str(offset));
    init->write();
}

void Return::write() const
{
    DataTree::actualOne()(RETURN);
    ret_val->write();
}

void ReturnNothing::write() const
{
    DataTree::actualOne()(RETURN_NOTHING);
}

void IntLiteral::write() const
{
    DataTree::actualOne()(INTEGER, util::str(value));
}

void FloatLiteral::write() const
{
    DataTree::actualOne()(FLOATING, util::str(value));
}

void BoolLiteral::write() const
{
    DataTree::actualOne()(BOOLEAN, util::str(value));
}

void StringLiteral::write() const
{
    DataTree::actualOne()(STRING, value);
}

void EmptyListLiteral::write() const
{
    DataTree::actualOne()(EMPTY_LIST);
}

static void writeList(std::vector<util::sptr<Expression const>> const& list)
{
    std::for_each(list.begin()
                , list.end()
                , [&](util::sptr<Expression const> const& member)
                  {
                      member->write();
                  });
}

void ListLiteral::write() const
{
    DataTree::actualOne()(LIST_BEGIN);
    writeList(value);
    DataTree::actualOne()(LIST_END);
}

void ListElement::write() const
{
    DataTree::actualOne()(LIST_ELEMENT);
}

void ListIndex::write() const
{
    DataTree::actualOne()(LIST_INDEX);
}

void Reference::write() const
{
    DataTree::actualOne()(REFERENCE);
}

void Call::write() const
{
    DataTree::actualOne()(CALL, util::str(int(args.size())));
    writeList(args);
}

void FuncReference::write() const
{
    DataTree::actualOne()(FUNC_REFERENCE);
}

void ListAppend::write() const
{
    DataTree::actualOne()(BINARY_OP, "++");
    lhs->write();
    rhs->write();
}

void BinaryOp::write() const
{
    DataTree::actualOne()(BINARY_OP, op);
    lhs->write();
    rhs->write();
}

void PreUnaryOp::write() const
{
    DataTree::actualOne()(PRE_UNARY_OP, op);
    rhs->write();
}

void Conjunction::write() const
{
    DataTree::actualOne()(CONJUNCTION);
    lhs->write();
    rhs->write();
}

void Disjunction::write() const
{
    DataTree::actualOne()(DISJUNCTION);
    lhs->write();
    rhs->write();
}

void Negation::write() const
{
    DataTree::actualOne()(NEGATION);
    rhs->write();
}

void WriterExpr::write() const
{
    DataTree::actualOne()(WRITER);
}

void MemberCall::write() const
{
    DataTree::actualOne()(MEMBER_CALL, name, args.size());
    object->write();
    writeList(args);
}

void ListPipeline::write() const
{
    DataTree::actualOne()(LIST_PIPELINE, util::str(int(pipeline.size())));
    list->write();
    std::for_each(pipeline.begin()
                , pipeline.end()
                , [&](util::sptr<PipeBase const> const& pipe)
                  {
                      pipe->begin();
                  });
}

void PipeMap::begin() const
{
    DataTree::actualOne()(PIPE_MAP);
}

void PipeFilter::begin() const
{
    DataTree::actualOne()(PIPE_FILTER);
}

void Function::writeDecl() const
{
    DataTree::actualOne()(FUNC_DECL, util::str(level), params.size());
}

void Function::writeImpl() const
{
    DataTree::actualOne()(FUNC_IMPL, util::str(level), params.size());
    body->write();
}

std::string VoidPrimitive::exportedName() const
{
    return "";
}

std::string IntPrimitive::exportedName() const
{
    return "";
}

std::string FloatPrimitive::exportedName() const
{
    return "";
}

std::string BoolPrimitive::exportedName() const
{
    return "";
}

std::string StringPrimitive::exportedName() const
{
    return "";
}

std::string ClosureType::exportedName() const
{
    return "";
}

std::string ListType::exportedName() const
{
    return "";
}

std::string EmptyListType::exportedName() const
{
    return "";
}

void Type::writeResEntry(int) const {}
void ResourceType::writeResEntry(int) const {}

util::sptr<output::StackVarRec const> Type::makeParameter(Address const&) const
{
    return util::sptr<output::StackVarRec const>(nullptr);
}

util::sptr<output::StackVarRec const> ResourceType::makeParameter(Address const&) const
{
    return util::sptr<output::StackVarRec const>(nullptr);
}

void Expression::writePipeDef(int) const {}
void ListLiteral::writePipeDef(int) const {}
void Call::writePipeDef(int) const {}
void MemberCall::writePipeDef(int) const {}
void ListAppend::writePipeDef(int) const {}
void BinaryOp::writePipeDef(int) const {}
void PreUnaryOp::writePipeDef(int) const {}
void Conjunction::writePipeDef(int) const {}
void Disjunction::writePipeDef(int) const {}
void Negation::writePipeDef(int) const {}
void ListPipeline::writePipeDef(int) const {}
void PipeMap::end() const {}
void PipeFilter::end() const {}
void PipeMap::writeDef(int) const {}
void PipeFilter::writeDef(int) const {}
