#include "data-node.h"

using namespace test;

bool data_node_base::operator==(data_node_base const& rhs) const
{
    return pos == rhs.pos && type_id == rhs.type_id && indent_level == rhs.indent_level && cmp(rhs);
}

std::ostream& data_node_base::print(std::ostream& os) const
{
    return -1 == indent_level ? (os << pos << " no indent") : (os << pos << " indent level: " << indent_level);
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
    return node.print(os);
}
