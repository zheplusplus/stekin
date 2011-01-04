#include "data-node.h"

using namespace test;

bool data_node_base::operator==(data_node_base const& rhs) const
{
    return pos == rhs.pos && type_img == rhs.type_img && indent_level == rhs.indent_level && cmp(rhs);
}

bool data_node_base::cmp_no_data(nothing_node const&) const
{
    return false;
}

bool data_node_base::cmp_str_data(string_node const&) const
{
    return false;
}

std::ostream& nothing_node::print(std::ostream& os) const
{
    return os;
}

bool nothing_node::cmp(data_node_base const& rhs) const
{
    return rhs.cmp_no_data(*this);
}

bool nothing_node::cmp_no_data(nothing_node const&) const
{
    return true;
}

std::ostream& string_node::print(std::ostream& os) const
{
    return os << " string data: " << data;
}

bool string_node::cmp(data_node_base const& rhs) const
{
    return rhs.cmp_str_data(*this);
}

bool string_node::cmp_str_data(string_node const& lhs) const
{
    return data == lhs.data;
}

std::ostream& operator<<(std::ostream& os, data_node_base const& node)
{
    return node.print((-1 == node.indent_level ? (os << node.pos << " no indent")
                                               : (os << node.pos << " indent level: " << node.indent_level))
                        << " node type: " << node.type_img.type_img);
}

data_node_base::node_type const data_node_base::INTEGER("integer");
data_node_base::node_type const data_node_base::FLOATING("floating");
data_node_base::node_type const data_node_base::IDENTIFIER("identifier");

data_node_base::node_type const data_node_base::BINARY_OP_BEGIN("binary operation begin");
data_node_base::node_type const data_node_base::BINARY_OP_END("binary operation end");
data_node_base::node_type const data_node_base::PRE_UNARY_OP_BEGIN("prefix unary operation begin");
data_node_base::node_type const data_node_base::PRE_UNARY_OP_END("prefix unary operation end");
data_node_base::node_type const data_node_base::OPERAND("operand");

data_node_base::node_type const data_node_base::FUNC_CALL_BEGIN("func call begin");
data_node_base::node_type const data_node_base::FUNC_CALL_END("func call end");
data_node_base::node_type const data_node_base::ARGUMENT("argument");

data_node_base::node_type const data_node_base::VAR_DEF("var def");
data_node_base::node_type const data_node_base::ARITHMETICS("arithmetics");
data_node_base::node_type const data_node_base::RETURN("return");

data_node_base::node_type const data_node_base::FUNC_DEF_HEAD_BEGIN("func def head begin");
data_node_base::node_type const data_node_base::FUNC_DEF_HEAD_END("func def head end");

data_node_base::node_type const data_node_base::CONDITION_BEGIN("condition begin");
data_node_base::node_type const data_node_base::CONDITION_END("condition end");

data_node_base::node_type const data_node_base::BRANCH_IF("branch if");
data_node_base::node_type const data_node_base::BRANCH_IFNOT("branch ifnot");
data_node_base::node_type const data_node_base::BRANCH_ELSE("branch else");

data_node_base::node_type const data_node_base::LOOP_WHILE("loop while");
