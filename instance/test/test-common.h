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

        InstanceData(int l, int s_o)
            : level(l)
            , stack_size_or_offset(s_o)
        {}

        explicit InstanceData()
            : level(-1)
            , stack_size_or_offset(-1)
        {}

        bool operator==(InstanceData const& rhs) const
        {
            return level == rhs.level && stack_size_or_offset == rhs.stack_size_or_offset;
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

    struct InstanceTest
        : public testing::Test
    {
        void SetUp();
        void TearDown();
    };

}

std::ostream& operator<<(std::ostream& os, test::InstanceData const& data);

#endif /* __STEKIN_INSTANCE_TEST_TEST_COMMON_H__ */
