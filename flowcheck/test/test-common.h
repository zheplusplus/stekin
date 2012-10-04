#ifndef __STEKIN_FLOWCHECK_TEST_TEST_COMMON_H__
#define __STEKIN_FLOWCHECK_TEST_TEST_COMMON_H__

#include <proto/node-base.h>
#include <proto/block.h>
#include <proto/func-reference-type.h>
#include <proto/variable.h>
#include <misc/pos-type.h>
#include <test/data-node.h>
#include <test/data-trees.h>

namespace test {

    extern util::sref<proto::FuncInstDraft> const nul_draft;
    extern util::sref<proto::SymbolTable> const nul_st;
    extern misc::trace nultrace;

    void instBlock(util::sref<proto::Block> block);

    struct FlowcheckData {
        misc::position const pos;
        int const func_arg_size;
        bool const func_hint_ret_void;

        FlowcheckData(misc::position const& ps, int arg_size, bool hint_ret_void)
            : pos(ps)
            , func_arg_size(arg_size)
            , func_hint_ret_void(hint_ret_void)
        {}

        FlowcheckData(misc::position const& ps, int list_size)
            : pos(ps)
            , func_arg_size(list_size)
            , func_hint_ret_void(false)
        {}

        explicit FlowcheckData(misc::position const ps)
            : pos(ps)
            , func_arg_size(-1)
            , func_hint_ret_void(false)
        {}

        FlowcheckData()
            : func_arg_size(-1)
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
        DataTree& operator()(misc::position const& pos, NodeType const& type, int list_size);
    };

    extern NodeType const BOOLEAN;
    extern NodeType const INTEGER;
    extern NodeType const FLOATING;
    extern NodeType const STRING;
    extern NodeType const LIST;
    extern NodeType const BINARY_OP;
    extern NodeType const PRE_UNARY_OP;
    extern NodeType const CALL;
    extern NodeType const FUNC_REFERENCE;
    extern NodeType const FUNCTOR;
    extern NodeType const REFERENCE;
    extern NodeType const LIST_ELEMENT;
    extern NodeType const LIST_INDEX;

    extern NodeType const LIST_PIPELINE;
    extern NodeType const PIPE_MAP;
    extern NodeType const PIPE_FILTER;

    extern NodeType const VAR_DEF;

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
