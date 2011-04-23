#include "test-common.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"

using namespace test;

inst::BuiltInPrimitive const test::PROTO_TEST_TYPE("proto_test_type", 1);
inst::BuiltInPrimitive const test::PROTO_TEST_VOID("proto_test_void", 0);

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

NodeType const test::ADD_STMT_TO_SCOPE("add statement to scope");
NodeType const test::ADD_STMT_TO_BLOCK("add statement to block");

NodeType const test::QUERY_VAR("query var");
NodeType const test::QUERY_BINARY_OP("query binary op");
NodeType const test::QUERY_PRE_UNARY_OP("query pre-unary op");

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

NodeType const test::INIT_AS_VOID_RET("init as void return");
NodeType const test::SET_RETURN_TYPE_VOID("set return type void");
NodeType const test::SET_RETURN_TYPE("set return type");
NodeType const test::QUERY_RETURN_TYPE_RESOLVE_STATUS("is return type resolved");

NodeType const test::NEXT_PATH("inst next path");
NodeType const test::ADD_PATH("add path");

void ProtoTest::SetUp()
{
    clearErr();
}

void ProtoTest::TearDown()
{
    DataTree::verify();
}
