#include "test-common.h"
#include "phony-err-report.h"

using namespace test;

inst::type const test::PROTO_TEST_TYPE("proto_test_type", 1);

data_tree& data_tree::operator()(misc::pos_type const& pos, node_type const& type, std::string const& str)
{
    base_type::operator()(type, proto_data(pos), str);
    return *this;
}

data_tree& data_tree::operator()(misc::pos_type const& pos
                               , node_type const& type
                               , std::string const& str
                               , int func_arg_size)
{
    base_type::operator()(type, proto_data(pos, func_arg_size), str);
    return *this;
}

data_tree& data_tree::operator()(misc::pos_type const& pos, node_type const& type)
{
    base_type::operator()(type, proto_data(pos));
    return *this;
}

data_tree& data_tree::operator()(node_type const& type)
{
    base_type::operator()(type, proto_data());
    return *this;
}

data_tree& data_tree::operator()(node_type const& type, std::string const& str)
{
    base_type::operator()(type, proto_data(), str);
    return *this;
}

std::ostream& operator<<(std::ostream& os, proto_data const& data)
{
    return -1 == data.func_arg_size ? (os << data.pos) : (os << data.pos << " arg size=" << data.func_arg_size);
}

node_type const test::ADD_STMT("add statement");

node_type const test::QUERY_VAR("query var");
node_type const test::QUERY_BINARY_OP("query binary op");
node_type const test::QUERY_PRE_UNARY_OP("query pre-unary op");

node_type const test::BOOLEAN("boolean");
node_type const test::INTEGER("integer");
node_type const test::FLOATING("floating");
node_type const test::REFERENCE("reference");
node_type const test::BINARY_OP("binary operation");
node_type const test::PRE_UNARY_OP("prefix unary operation");
node_type const test::CALL("call");
node_type const test::CONJUNCTION("conjunction");
node_type const test::DISJUNCTION("disjunction");
node_type const test::NEGATION("negation");

void proto_test::SetUp()
{
    clear_err();
}

void proto_test::TearDown()
{
    data_tree::verify();
}
