#ifndef __STEKIN_INSTANCE_TEST_TEST_COMMON_H__
#define __STEKIN_INSTANCE_TEST_TEST_COMMON_H__

#include <gtest/gtest.h>

#include <test/common.h>
#include <test/data-node.h>
#include <test/data-trees.h>

namespace test {

    struct InstanceData {
        int const level;
        int const stack_size_or_offset;
        int const self_offset;

        InstanceData(int l, int s_o)
            : level(l)
            , stack_size_or_offset(s_o)
            , self_offset(-1)
        {}

        InstanceData()
            : level(-1)
            , stack_size_or_offset(-1)
            , self_offset(-1)
        {}

        InstanceData(int l, int o, int so)
            : level(l)
            , stack_size_or_offset(o)
            , self_offset(so)
        {}

        explicit InstanceData(int o)
            : level(-1)
            , stack_size_or_offset(o)
            , self_offset(-1)
        {}

        bool operator==(InstanceData const& rhs) const
        {
            return level == rhs.level
                && stack_size_or_offset == rhs.stack_size_or_offset
                && self_offset == rhs.self_offset;
        }
    };

    struct DataTree
        : public DataTreeTempl<InstanceData, DataTree>
    {
        typedef DataTreeTempl<InstanceData, DataTree> BaseType;

        DataTree& operator()(NodeType const& type
                           , std::string const& name
                           , int level
                           , int stack_size_or_offset);
        DataTree& operator()(NodeType const& type
                           , std::string const& name
                           , int level
                           , int offset
                           , int self_offset);
        DataTree& operator()(NodeType const& type, std::string const& name);
        DataTree& operator()(NodeType const& type);
        DataTree& operator()(NodeType const& type, std::string const& name, int offset);
        DataTree& operator()(NodeType const& type, int level, std::string const& name);
        DataTree& operator()(NodeType const& type, int offset);
    };

    extern std::string const PARAM;
    extern std::string const RES_PARAM;

    extern NodeType const FUNC_DECL_BEGIN;
    extern NodeType const FUNC_DECL_END;
    extern NodeType const FUNC_DEF;
    extern NodeType const PARAMETER;

    extern NodeType const BLOCK_BEGIN;
    extern NodeType const BLOCK_END;
    extern NodeType const END_OF_STATEMENT;

    extern NodeType const INTEGER;
    extern NodeType const FLOAT;
    extern NodeType const BOOLEAN;
    extern NodeType const EMPTY_LIST;
    extern NodeType const LIST_BEGIN;
    extern NodeType const LIST_NEXT_MEMBER;
    extern NodeType const LIST_END;

    extern NodeType const LIST_APPEND_BEGIN;
    extern NodeType const LIST_APPEND_END;

    extern NodeType const PIPE_MAP_BEGIN;
    extern NodeType const PIPE_MAP_END;
    extern NodeType const PIPE_FILTER_BEGIN;
    extern NodeType const PIPE_FILTER_END;
    extern NodeType const PIPE_BEGIN;
    extern NodeType const PIPE_END;
    extern NodeType const PIPE_ELEMENT;
    extern NodeType const PIPE_INDEX;

    extern NodeType const INITIALIZE_THIS_LEVEL;
    extern NodeType const REFERENCE;
    extern NodeType const CALL_BEGIN;
    extern NodeType const CALL_END;
    extern NodeType const ARG_SEPARATOR;
    extern NodeType const FUNC_REFERENCE;
    extern NodeType const FUNC_REF_NEXT_VAR;
    extern NodeType const FUNC_RES_ENTRY;
    extern NodeType const ADD_RES_ENTRY;

    extern NodeType const OPERATOR;
    extern NodeType const EXPRESSION_BEGIN;
    extern NodeType const EXPRESSION_END;

    extern NodeType const WRITER_BEGIN;
    extern NodeType const WRITER_END;
    extern NodeType const MEMBER_CALL_BEGIN;
    extern NodeType const MEMBER_CALL_END;

    extern NodeType const BRANCH_IF;
    extern NodeType const BRANCH_ELSE;

    extern NodeType const RETURN;
    extern NodeType const RETURN_NOTHING;

    struct InstanceTest
        : public testing::Test
    {
        void TearDown();
    };

}

std::ostream& operator<<(std::ostream& os, test::InstanceData const& data);

#endif /* __STEKIN_INSTANCE_TEST_TEST_COMMON_H__ */
