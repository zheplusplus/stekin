#include "test-common.h"
#include "../../test/common.h"

using namespace test;

data_tree& data_tree::operator()(misc::position const& pos, node_type const& type, std::string const& str)
{
    base_type::operator()(type, parser_data(pos), str);
    return *this;
}

data_tree& data_tree::operator()(misc::position const& pos
                               , int indent
                               , node_type const& type
                               , std::string const& str)
{
    base_type::operator()(type, parser_data(indent, pos), str);
    return *this;
}

data_tree& data_tree::operator()(misc::position const& pos, node_type const& type)
{
    base_type::operator()(type, parser_data(pos));
    return *this;
}

data_tree& data_tree::operator()(misc::position const& pos, int indent, node_type const& type)
{
    base_type::operator()(type, parser_data(indent, pos));
    return *this;
}

std::ostream& operator<<(std::ostream& os, parser_data const& data)
{
    return (-1 == data.indent_level ? os << "no indent " : os << "indent level: " << data.indent_level << " ")
                        << data.pos;
}

node_type const test::BOOLEAN("boolean");
node_type const test::INTEGER("integer");
node_type const test::FLOATING("floating");
node_type const test::IDENTIFIER("identifier");

node_type const test::BINARY_OP_BEGIN("binary operation begin");
node_type const test::BINARY_OP_END("binary operation end");
node_type const test::PRE_UNARY_OP_BEGIN("prefix unary operation begin");
node_type const test::PRE_UNARY_OP_END("prefix unary operation end");
node_type const test::OPERAND("operand");

node_type const test::FUNC_CALL_BEGIN("func call begin");
node_type const test::FUNC_CALL_END("func call end");
node_type const test::ARGUMENT("argument");

node_type const test::VAR_DEF("var def");
node_type const test::ARITHMETICS("arithmetics");
node_type const test::RETURN("return");

node_type const test::FUNC_DEF_HEAD_BEGIN("func def head begin");
node_type const test::FUNC_DEF_HEAD_END("func def head end");

node_type const test::CONDITION_BEGIN("condition begin");
node_type const test::CONDITION_END("condition end");

node_type const test::BRANCH_IF("branch if");
node_type const test::BRANCH_IFNOT("branch ifnot");
node_type const test::BRANCH_ELSE("branch else");
