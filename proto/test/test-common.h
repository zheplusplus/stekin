#ifndef __STACKENING_PROTO_TEST_TEST_COMMON_H__
#define __STACKENING_PROTO_TEST_TEST_COMMON_H__

#include "../../instance/type.h"
#include "../../misc/pos-type.h"
#include "../../test/data-node.h"
#include "../../test/data-trees.h"

namespace test {

    extern inst::type const PROTO_TEST_TYPE;

    struct proto_data {
        misc::pos_type const pos;
        int const func_arg_size;

        proto_data()
            : pos(-1)
            , func_arg_size(-1)
        {}

        explicit proto_data(misc::pos_type const ps)
            : pos(ps)
            , func_arg_size(-1)
        {}

        proto_data(misc::pos_type const& ps, int arg_size)
            : pos(ps)
            , func_arg_size(arg_size)
        {}

        bool operator==(proto_data const& rhs) const
        {
            return pos == rhs.pos && func_arg_size == rhs.func_arg_size;
        }
    };

    typedef data_node_templ<proto_data> data_node_base;
    typedef nothing_node_templ<proto_data> nothing_node;
    typedef string_node_templ<proto_data> string_node;

    struct data_tree
        : public data_tree_templ<proto_data, data_tree>
    {
        typedef data_tree_templ<proto_data, data_tree> base_type;

        data_tree& operator()(misc::pos_type const& pos, node_type const& type, std::string const& str);
        data_tree& operator()(misc::pos_type const& pos
                            , node_type const& type
                            , std::string const& str
                            , int func_arg_size);
        data_tree& operator()(misc::pos_type const& pos, node_type const& type);

        data_tree& operator()(node_type const& type);
        data_tree& operator()(node_type const& type, std::string const& str);
    };

    extern node_type const ADD_STMT;

    extern node_type const QUERY_VAR;
    extern node_type const QUERY_BINARY_OP;
    extern node_type const QUERY_PRE_UNARY_OP;

    extern node_type const BOOLEAN;
    extern node_type const INTEGER;
    extern node_type const FLOATING;
    extern node_type const REFERENCE;
    extern node_type const BINARY_OP;
    extern node_type const PRE_UNARY_OP;
    extern node_type const CALL;
    extern node_type const CONJUNCTION;
    extern node_type const DISJUNCTION;
    extern node_type const NEGATION;

    struct proto_test
        : public testing::Test
    {
        void SetUp();
        void TearDown();
    };

}

std::ostream& operator<<(std::ostream& os, test::proto_data const& data);

#endif /* __STACKENING_PROTO_TEST_TEST_COMMON_H__ */