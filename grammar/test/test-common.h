#ifndef __STEKIN_GRAMMAR_TEST_TEST_COMMON_H__
#define __STEKIN_GRAMMAR_TEST_TEST_COMMON_H__

#include "../node-base.h"
#include "../../flowcheck/node-base.h"
#include "../../misc/pos-type.h"
#include "../../test/data-node.h"
#include "../../test/data-trees.h"

namespace test {

    extern util::sref<proto::scope> const nulscope;

    struct grammar_data {
        misc::position const pos;
        int const func_arg_size;

        grammar_data(misc::position const& ps, int arg_size)
            : pos(ps)
            , func_arg_size(arg_size)
        {}

        explicit grammar_data(misc::position const ps)
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

    struct DataTree
        : public DataTreeTempl<grammar_data, DataTree>
    {
        typedef DataTreeTempl<grammar_data, DataTree> BaseType;

        DataTree& operator()(misc::position const& pos, NodeType const& type, std::string const& str);
        DataTree& operator()(misc::position const& pos
                            , NodeType const& type
                            , std::string const& str
                            , int func_arg_size);
        DataTree& operator()(NodeType const& type);
        DataTree& operator()(misc::position const& pos, NodeType const& type);
    };

    extern std::string const VAR_DEF_FILTERED;
    extern std::string const FUNC_DEF_FILTERED;

    extern NodeType const BOOLEAN;
    extern NodeType const INTEGER;
    extern NodeType const FLOATING;
    extern NodeType const REFERENCE;

    extern NodeType const BINARY_OP;
    extern NodeType const PRE_UNARY_OP;

    extern NodeType const CALL;
    extern NodeType const FUNC_REFERENCE;

    extern NodeType const VAR_DEF;
    extern NodeType const ARITHMETICS;
    extern NodeType const RETURN;
    extern NodeType const RETURN_NOTHING;

    extern NodeType const FUNC_DEF;
    extern NodeType const PARAMETER;

    extern NodeType const BRANCH;
    extern NodeType const BRANCH_CONSQ_ONLY;
    extern NodeType const BRANCH_ALTER_ONLY;
    extern NodeType const CONSEQUENCE;
    extern NodeType const ALTERNATIVE;

    extern NodeType const BLOCK_BEGIN;
    extern NodeType const BLOCK_END;

    struct grammar_test
        : public testing::Test
    {
        void SetUp();
        void TearDown();
    };

}

std::ostream& operator<<(std::ostream& os, test::grammar_data const& data);

#endif /* __STEKIN_GRAMMAR_TEST_TEST_COMMON_H__ */
