#ifndef __STACKENING_TEST_DATA_TREES_H__
#define __STACKENING_TEST_DATA_TREES_H__

#include <list>
#include <string>
#include <ostream>
#include <algorithm>
#include <gtest/gtest.h>

#include "data-node.h"
#include "../util/pointer.h"

namespace test {

    template <typename _NodeData, typename _FinalType>
    struct data_tree_templ {
        data_tree_templ& operator()(node_type const& type, _NodeData const& sub, std::string const& str)
        {
            _nodes.push_back(std::move(util::mkptr(new string_node_templ<_NodeData>(type, sub, str))));
            return *this;
        }

        data_tree_templ& operator()(node_type const& type, _NodeData const& sub)
        {
            _nodes.push_back(std::move(util::mkptr(new nothing_node_templ<_NodeData>(type, sub))));
            return *this;
        }
    protected:
        data_tree_templ() {}
    public:
        static _FinalType& expect_one()
        {
            static _FinalType expect;
            return expect;
        }

        static _FinalType& actual_one()
        {
            static _FinalType actual;
            return actual;
        }

        static void verify()
        {
            ASSERT_EQ(expect_one()._nodes.size(), actual_one()._nodes.size());
            auto expect_iter = expect_one()._nodes.begin();
            auto actual_iter = actual_one()._nodes.begin();
            for (; expect_one()._nodes.end() != expect_iter; ++expect_iter, ++actual_iter) {
                EXPECT_EQ(**expect_iter, **actual_iter);
            }
            expect_one()._nodes.clear();
            actual_one()._nodes.clear();
        }
    private:
        std::list<util::sptr<data_node_templ<_NodeData> const>> _nodes;
    };

}

#endif /* __STACKENING_TEST_DATA_TREES_H__ */
