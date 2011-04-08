#ifndef __STACKENING_FLOW_CHECK_TEST_TEST_COMMON_H__
#define __STACKENING_FLOW_CHECK_TEST_TEST_COMMON_H__

#include "../../proto/node-base.h"
#include "../../proto/scope.h"
#include "../../misc/pos-type.h"
#include "../../test/data-node.h"
#include "../../test/data-trees.h"

namespace test {

    extern util::sref<inst::scope> const nul_inst_scope;

    struct flowcheck_data {
        misc::pos_type const pos;
        int const func_arg_size;
        bool const func_hint_ret_void;

        flowcheck_data(misc::pos_type const& ps, int arg_size, bool hint_ret_void)
            : pos(ps)
            , func_arg_size(arg_size)
            , func_hint_ret_void(hint_ret_void)
        {}

        explicit flowcheck_data(misc::pos_type const ps)
            : pos(ps)
            , func_arg_size(-1)
            , func_hint_ret_void(false)
        {}

        flowcheck_data()
            : pos(-1)
            , func_arg_size(-1)
            , func_hint_ret_void(false)
        {}

        bool operator==(flowcheck_data const& rhs) const
        {
            return pos == rhs.pos
                && func_arg_size == rhs.func_arg_size
                && func_hint_ret_void == rhs.func_hint_ret_void;
        }
    };

    typedef data_node_templ<flowcheck_data> data_node_base;
    typedef nothing_node_templ<flowcheck_data> nothing_node;
    typedef string_node_templ<flowcheck_data> string_node;

    struct data_tree
        : public data_tree_templ<flowcheck_data, data_tree>
    {
        typedef data_tree_templ<flowcheck_data, data_tree> base_type;

        data_tree& operator()(misc::pos_type const& pos, node_type const& type, std::string const& str);
        data_tree& operator()(misc::pos_type const& pos
                            , node_type const& type
                            , std::string const& str
                            , int func_arg_size
                            , bool func_hint_return_void);
        data_tree& operator()(node_type const& type);
        data_tree& operator()(misc::pos_type const& pos, node_type const& type);
    };

    extern node_type const BOOLEAN;
    extern node_type const INTEGER;
    extern node_type const FLOATING;
    extern node_type const BINARY_OP;
    extern node_type const PRE_UNARY_OP;
    extern node_type const CALL;
    extern node_type const FUNCTOR;
    extern node_type const FUNC_REFERENCE;

    extern node_type const VAR_REF;
    extern node_type const VAR_DEF;

    extern node_type const STATEMENT;
    extern node_type const ARITHMETICS;
    extern node_type const RETURN;
    extern node_type const RETURN_NOTHING;

    extern node_type const FUNC_DECL;
    extern node_type const PARAMETER;

    extern node_type const SCOPE_BEGIN;
    extern node_type const SCOPE_END;

    extern node_type const BRANCH;

    struct flowcheck_test
        : public testing::Test
    {
        void SetUp();
        void TearDown();
    };

}

std::ostream& operator<<(std::ostream& os, test::flowcheck_data const& data);

#endif /* __STACKENING_FLOW_CHECK_TEST_TEST_COMMON_H__ */
