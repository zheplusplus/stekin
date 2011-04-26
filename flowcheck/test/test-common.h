#ifndef __STEKIN_FLOWCHECK_TEST_TEST_COMMON_H__
#define __STEKIN_FLOWCHECK_TEST_TEST_COMMON_H__

#include "../../proto/node-base.h"
#include "../../proto/scope.h"
#include "../../misc/pos-type.h"
#include "../../test/data-node.h"
#include "../../test/data-trees.h"

namespace test {

    extern util::sref<inst::Scope> const nul_inst_scope;

    struct FlowcheckData {
        misc::position const pos;
        int const func_arg_size;
        bool const func_hint_ret_void;

        FlowcheckData(misc::position const& ps, int arg_size, bool hint_ret_void)
            : pos(ps)
            , func_arg_size(arg_size)
            , func_hint_ret_void(hint_ret_void)
        {}

        explicit FlowcheckData(misc::position const ps)
            : pos(ps)
            , func_arg_size(-1)
            , func_hint_ret_void(false)
        {}

        FlowcheckData()
            : pos(-1)
            , func_arg_size(-1)
            , func_hint_ret_void(false)
        {}

        bool operator==(FlowcheckData const& rhs) const
        {
            return pos == rhs.pos
                && func_arg_size == rhs.func_arg_size
                && func_hint_ret_void == rhs.func_hint_ret_void;
        }
    };

    struct DataTree
        : public DataTreeTempl<FlowcheckData, DataTree>
    {
        typedef DataTreeTempl<FlowcheckData, DataTree> BaseType;

        DataTree& operator()(misc::position const& pos
                           , NodeType const& type
                           , std::string const& str);
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

    struct FlowcheckTest
        : public testing::Test
    {
        void SetUp();
        void TearDown();
    };

}

std::ostream& operator<<(std::ostream& os, test::FlowcheckData const& data);

#endif /* __STEKIN_FLOWCHECK_TEST_TEST_COMMON_H__ */
