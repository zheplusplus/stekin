#include "test-common.h"

using namespace test;

data_tree& data_tree::operator()(misc::pos_type const& pos, node_type const& type, std::string const& str)
{
    base_type::operator()(type, grammar_data(pos), str);
    return *this;
}

data_tree& data_tree::operator()(misc::pos_type const& pos
                               , int indent
                               , node_type const& type
                               , std::string const& str)
{
    base_type::operator()(type, grammar_data(indent, pos), str);
    return *this;
}

data_tree& data_tree::operator()(misc::pos_type const& pos, node_type const& type)
{
    base_type::operator()(type, grammar_data(pos));
    return *this;
}

data_tree& data_tree::operator()(misc::pos_type const& pos
                               , node_type const& type
                               , std::string const& str
                               , int func_arg_size)
{
    base_type::operator()(type, grammar_data(pos, func_arg_size), str);
    return *this;
}

data_tree& data_tree::operator()(node_type const& type)
{
    base_type::operator()(type, grammar_data());
    return *this;
}

data_tree& data_tree::operator()(misc::pos_type const& pos, int indent, node_type const& type)
{
    base_type::operator()(type, grammar_data(indent, pos));
    return *this;
}

std::ostream& operator<<(std::ostream& os, grammar_data const& data)
{
    return (-1 == data.indent_level ? os << "no indent " : os << "indent level: " << data.indent_level << " ")
                        << data.pos;
}

node_type const test::BOOLEAN("boolean");
node_type const test::INTEGER("integer");
node_type const test::FLOATING("floating");
node_type const test::REFERENCE("reference");

node_type const test::BINARY_OP("binary operation");
node_type const test::PRE_UNARY_OP("prefix unary operation");

node_type const test::CALL("call");

node_type const test::VAR_DEF("var def");
node_type const test::ARITHMETICS("arithmetics");
node_type const test::RETURN("return");
node_type const test::RETURN_NOTHING("return nothing");

node_type const test::FUNC_DECL("func decl");
node_type const test::PARAMETER("parameter");

node_type const test::SCOPE("scope");
node_type const test::BRANCH("branch");
node_type const test::LOOP("loop");
