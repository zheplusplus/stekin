#ifndef __STACKENING_GRAMMAR_TEST_TEST_COMMON_H__
#define __STACKENING_GRAMMAR_TEST_TEST_COMMON_H__

#include "../node-base.h"
#include "../../flowcheck/node-base.h"
#include "../../misc/pos-type.h"
#include "../../test/data-node.h"
#include "../../test/data-trees.h"

namespace test {

    struct grammar_data {
        misc::pos_type const pos;
        int const func_arg_size;

        grammar_data(misc::pos_type const& ps, int arg_size)
            : pos(ps)
            , func_arg_size(arg_size)
        {}

        explicit grammar_data(misc::pos_type const ps)
            : pos(ps)
            , func_arg_size(-1)
        {}

        grammar_data()
            : pos(-1)
            , func_arg_size(-1)
        {}

        bool operator==(grammar_data const& rhs) const
        {
            return pos == rhs.pos && func_arg_size == rhs.func_arg_size;
        }
    };

    typedef data_node_templ<grammar_data> data_node_base;
    typedef nothing_node_templ<grammar_data> nothing_node;
    typedef string_node_templ<grammar_data> string_node;

    struct data_tree
        : public data_tree_templ<grammar_data, data_tree>
    {
        typedef data_tree_templ<grammar_data, data_tree> base_type;

        data_tree& operator()(misc::pos_type const& pos, node_type const& type, std::string const& str);
        data_tree& operator()(misc::pos_type const& pos
                            , node_type const& type
                            , std::string const& str
                            , int func_arg_size);
        data_tree& operator()(node_type const& type);
        data_tree& operator()(misc::pos_type const& pos, node_type const& type);
    };

    extern node_type const BOOLEAN;
    extern node_type const INTEGER;
    extern node_type const FLOATING;
    extern node_type const REFERENCE;

    extern node_type const BINARY_OP;
    extern node_type const PRE_UNARY_OP;

    extern node_type const CALL;
    extern node_type const FUNC_REFERENCE;

    extern node_type const VAR_DEF;
    extern node_type const VAR_DEF_FILTERED;
    extern node_type const ARITHMETICS;
    extern node_type const RETURN;
    extern node_type const RETURN_NOTHING;

    extern node_type const FUNC_DEF;
    extern node_type const FUNC_DEF_FILTERED;
    extern node_type const PARAMETER;

    extern node_type const BRANCH;
    extern node_type const BRANCH_CONSQ_ONLY;
    extern node_type const BRANCH_ALTER_ONLY;

    struct grammar_test
        : public testing::Test
    {
        void SetUp();
        void TearDown();
    };

}

std::ostream& operator<<(std::ostream& os, test::grammar_data const& data);

#endif /* __STACKENING_GRAMMAR_TEST_TEST_COMMON_H__ */
