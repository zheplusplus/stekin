#include <algorithm>
#include <gtest/gtest.h>

#include "data-trees.h"

using namespace test;

data_tree& data_tree::expect_one()
{
    static data_tree expect;
    return expect;
}

data_tree& data_tree::actual_one()
{
    static data_tree actual;
    return actual;
}

void data_tree::verify()
{
    ASSERT_EQ(expect_one()._nodes.size(), actual_one()._nodes.size());
    auto expect_iter = expect_one()._nodes.begin();
    auto actual_iter = actual_one()._nodes.begin();
    for (; expect_one()._nodes.end() != expect_iter; ++expect_iter, ++actual_iter) {
        EXPECT_EQ(*expect_iter, *actual_iter);
    }
}

data_tree& data_tree::operator()(misc::pos_type const& pos
                               , data_node_base::node_type type
                               , std::string const& data)
{
    _nodes.push_back(std::move(util::sptr<data_node_base const>(new string_node(pos, type, data))));
    return *this;
}

data_tree& data_tree::operator()(misc::pos_type const& pos
                               , int indent
                               , data_node_base::node_type type
                               , std::string const& data)
{
    _nodes.push_back(std::move(util::sptr<data_node_base const>(new string_node(pos, indent, type, data))));
    return *this;
}

data_tree& data_tree::operator()(misc::pos_type const& pos, data_node_base::node_type type)
{
    _nodes.push_back(std::move(util::sptr<data_node_base const>(new nothing_node(pos, type))));
    return *this;
}

data_tree& data_tree::operator()(misc::pos_type const& pos, int indent, data_node_base::node_type type)
{
    _nodes.push_back(std::move(util::sptr<data_node_base const>(new nothing_node(pos, indent, type))));
    return *this;
}

std::ostream& operator<<(std::ostream& os, util::sptr<data_node_base const> const& node)
{
    return os << *node;
}
