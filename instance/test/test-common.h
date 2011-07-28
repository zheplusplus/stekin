#ifndef __STEKIN_INSTANCE_TEST_TEST_COMMON_H__
#define __STEKIN_INSTANCE_TEST_TEST_COMMON_H__

#include <gtest/gtest.h>

#include "../node-base.h"
#include "../../test/common.h"
#include "../../test/data-node.h"
#include "../../test/data-trees.h"

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

        explicit InstanceData()
            : level(-1)
            , stack_size_or_offset(-1)
            , self_offset(-1)
        {}

        InstanceData(int l, int o, int so)
            : level(l)
            , stack_size_or_offset(o)
            , self_offset(so)
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
        DataTree& operator()(NodeType const& type, std::string const& str, int func_arg_size);
    };

    extern NodeType const FUNC_DECL_BEGIN;
    extern NodeType const FUNC_DECL_END;
    extern NodeType const FUNC_DEF;
    extern NodeType const PARAMETER;

    extern NodeType const BLOCK_BEGIN;
    extern NodeType const BLOCK_END;

    extern NodeType const INTEGER;
    extern NodeType const FLOAT;
    extern NodeType const BOOLEAN;

    extern NodeType const REFERENCE;
    extern NodeType const CALL_BEGIN;
    extern NodeType const CALL_END;
    extern NodeType const ARG_SEPARATOR;
    extern NodeType const FUNC_REFERENCE;
    extern NodeType const FUNC_REF_NEXT_VAR;

    extern NodeType const OPERATOR;
    extern NodeType const EXPRESSION_BEGIN;
    extern NodeType const EXPRESSION_END;

    extern NodeType const WRITE_BEGIN;
    extern NodeType const WRITE_END;

    struct InstanceTest
        : public testing::Test
    {
        void SetUp();
        void TearDown();
    };

}

std::ostream& operator<<(std::ostream& os, test::InstanceData const& data);

#endif /* __STEKIN_INSTANCE_TEST_TEST_COMMON_H__ */
