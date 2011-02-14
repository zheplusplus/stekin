#ifndef __STACKENING_INSTANCE_TEST_TEST_COMMON_H__
#define __STACKENING_INSTANCE_TEST_TEST_COMMON_H__

#include <gtest/gtest.h>

#include "../type.h"
#include "../node-base.h"

namespace test {

    extern inst::type const WORD;
    extern inst::type const DWORD;
    extern inst::type const HALFWORD;

    struct cond_type_expr
        : public inst::expr_base
    {
        inst::type const* typeof() const
        {
            return inst::type::BIT_BOOL;
        }

        void write() const {}
    };

    struct void_type_expr
        : public inst::expr_base
    {
        inst::type const* typeof() const
        {
            return inst::type::BIT_VOID;
        }

        void write() const {}
    };

    struct bad_type_expr
        : public inst::expr_base
    {
        inst::type const* typeof() const
        {
            return inst::type::BAD_TYPE;
        }

        void write() const {}
    };

    struct instance_test
        : public testing::Test
    {
        void SetUp();
    };

}

#endif /* __STACKENING_INSTANCE_TEST_TEST_COMMON_H__ */
