#ifndef __STEKIN_FLOW_CHECK_TEST_TEST_COMMON_H__
#define __STEKIN_FLOW_CHECK_TEST_TEST_COMMON_H__

#include "../../proto/node-base.h"
#include "../../proto/scope.h"
#include "../../misc/pos-type.h"
#include "../../test/data-node.h"
#include "../../test/data-trees.h"

namespace test {

    extern util::sref<inst::scope> const nul_inst_scope;

    struct flowcheck_data {
        misc::position const pos;
        int const func_arg_size;
        bool const func_hint_ret_void;

        flowcheck_data(misc::position const& ps, int arg_size, bool hint_ret_void)
            : pos(ps)
            , func_arg_size(arg_size)
            , func_hint_ret_void(hint_ret_void)
        {}

        explicit flowcheck_data(misc::position const ps)
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

    struct DataTree
        : public DataTreeTempl<flowcheck_data, DataTree>
    {
        typedef DataTreeTempl<flowcheck_data, DataTree> BaseType;

        DataTree& operator()(misc::position const& pos, NodeType const& type, std::string const& str);
        DataTree& operator()(misc::position const& pos
                            , NodeType const& type
                            , std::string const& str
                            , int func_arg_size
                            , bool func_hint_return_void);
        DataTree& operator()(NodeType const& type);
        DataTree& operator()(misc::position const& pos, NodeType const& type);
    };

    extern NodeType const BOOLEAN;
    extern NodeType const INTEGER;
    extern NodeType const FLOATING;
    extern NodeType const BINARY_OP;
    extern NodeType const PRE_UNARY_OP;
    extern NodeType const CALL;
    extern NodeType const FUNC_REFERENCE;
    extern NodeType const REFERENCE;

    extern NodeType const VAR_DEF;
    extern NodeType const SCOPE_VAR_DEF;

    extern NodeType const STATEMENT;
    extern NodeType const ARITHMETICS;
    extern NodeType const RETURN;
    extern NodeType const RETURN_NOTHING;

    extern NodeType const FUNC_DECL;
    extern NodeType const PARAMETER;

    extern NodeType const SCOPE_BEGIN;
    extern NodeType const SCOPE_END;

    extern NodeType const BRANCH;

    struct flowcheck_test
        : public testing::Test
    {
        void SetUp();
        void TearDown();
    };

}

std::ostream& operator<<(std::ostream& os, test::flowcheck_data const& data);

#endif /* __STEKIN_FLOW_CHECK_TEST_TEST_COMMON_H__ */
