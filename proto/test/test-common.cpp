#include "test-common.h"
#include "../stmt-nodes.h"
#include "../block.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"
#include "../../instance/node-base.h"

using namespace test;

DataTree& DataTree::operator()(misc::position const& pos
                             , NodeType const& type
                             , std::string const& str)
{
    BaseType::operator()(type, ProtoData(pos), str);
    return *this;
}

DataTree& DataTree::operator()(misc::position const& pos
                             , NodeType const& type
                             , std::string const& str
                             , int func_arg_size)
{
    BaseType::operator()(type, ProtoData(pos, func_arg_size), str);
    return *this;
}

DataTree& DataTree::operator()(misc::position const& pos, NodeType const& type)
{
    BaseType::operator()(type, ProtoData(pos));
    return *this;
}

DataTree& DataTree::operator()(NodeType const& type)
{
    BaseType::operator()(type, ProtoData());
    return *this;
}

DataTree& DataTree::operator()(NodeType const& type, std::string const& str)
{
    BaseType::operator()(type, ProtoData(), str);
    return *this;
}

DataTree& DataTree::operator()(NodeType const& type, std::string const& str, int func_arg_size)
{
    BaseType::operator()(type, ProtoData(func_arg_size), str);
    return *this;
}

std::ostream& operator<<(std::ostream& os, ProtoData const& data)
{
    return -1 == data.func_arg_size ? (os << data.pos)
                                    : (os << data.pos << " arg size=" << data.func_arg_size);
}

std::ostream& operator<<(std::ostream& os, proto::Variable const& var)
{
    os << "variable pos=" << var.def_pos
       << " type=" << var.type
       << " offset=" << var.stack_offset
       << " level=" << var.level;
    return os;
}

std::ostream& operator<<(std::ostream& os, util::sref<proto::Type const> type)
{
    return os << type->name();
}

NodeType const test::BLOCK_BEGIN("block begin");
NodeType const test::BLOCK_END("block end");
NodeType const test::ARITHMETICS("arithmetics");
NodeType const test::BRANCH("branch");
NodeType const test::INITIALIZATION("initialization");
NodeType const test::RETURN("return");
NodeType const test::RETURN_NOTHING("return nothing");

NodeType const test::BOOLEAN("boolean");
NodeType const test::INTEGER("integer");
NodeType const test::FLOATING("floating");
NodeType const test::REFERENCE("reference");
NodeType const test::BINARY_OP("binary operation");
NodeType const test::PRE_UNARY_OP("prefix unary operation");
NodeType const test::CALL("call");
NodeType const test::FUNC_REFERENCE("func reference");
NodeType const test::CONJUNCTION("conjunction");
NodeType const test::DISJUNCTION("disjunction");
NodeType const test::NEGATION("negation");
NodeType const test::WRITE("write");

NodeType const test::FUNC_DECL("function declaration");
NodeType const test::FUNC_IMPL("function implement");

void ProtoTest::SetUp()
{
    clearErr();
}

void ProtoTest::TearDown()
{
    DataTree::verify();
}

BlockFiller BlockFiller::branch(misc::position const& pos
                              , util::sptr<proto::Expression const> p
                              , util::sptr<proto::Block> c
                              , util::sptr<proto::Block> a) const
{
    _block->addStmt(util::mkptr(new proto::Branch(pos, std::move(p), std::move(c), std::move(a))));
    return *this;
}

BlockFiller BlockFiller::arith(misc::position const& pos
                             , util::sptr<proto::Expression const> e) const
{
    _block->addStmt(util::mkptr(new proto::Arithmetics(pos, std::move(e))));
    return *this;
}

BlockFiller BlockFiller::def(misc::position const& pos
                           , std::string const& name
                           , util::sptr<proto::Expression const> e) const
{
    _block->addStmt(util::mkptr(new proto::VarDef(pos, name, std::move(e))));
    return *this;
}

BlockFiller BlockFiller::ret(misc::position const& pos, util::sptr<proto::Expression const> e) const
{
    _block->addStmt(util::mkptr(new proto::Return(pos, std::move(e))));
    return *this;
}

BlockFiller BlockFiller::ret(misc::position const& pos) const
{
    _block->addStmt(util::mkptr(new proto::ReturnNothing(pos)));
    return *this;
}
