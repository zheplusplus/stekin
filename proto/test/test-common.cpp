#include "test-common.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"

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

void ProtoTest::SetUp()
{
    clearErr();
}

void ProtoTest::TearDown()
{
    DataTree::verify();
}
