#ifndef __STEKIN_INSTANCE_TEST_TEST_COMMON_H__
#define __STEKIN_INSTANCE_TEST_TEST_COMMON_H__

#include <gtest/gtest.h>

#include "../node-base.h"

namespace test {

    struct InstanceTest
        : public testing::Test
    {
        void SetUp();
    };

}

#endif /* __STEKIN_INSTANCE_TEST_TEST_COMMON_H__ */
