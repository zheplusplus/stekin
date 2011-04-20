#ifndef __STEKIN_INSTANCE_TEST_TEST_COMMON_H__
#define __STEKIN_INSTANCE_TEST_TEST_COMMON_H__

#include <gtest/gtest.h>

#include "../type.h"
#include "../node-base.h"

namespace test {

    extern inst::built_in_primitive const WORD;
    extern inst::built_in_primitive const DWORD;
    extern inst::built_in_primitive const HALFWORD;

    struct cond_type_expr
        : public inst::expr_base
    {
        util::sref<inst::type const> typeof() const
        {
            return inst::type::BIT_BOOL;
        }

        void write() const {}
    };

    struct void_type_expr
        : public inst::expr_base
    {
        util::sref<inst::type const> typeof() const
        {
            return inst::type::BIT_VOID;
        }

        void write() const {}
    };

    struct bad_type_expr
        : public inst::expr_base
    {
        util::sref<inst::type const> typeof() const
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

std::ostream& operator<<(std::ostream& os, inst::variable const& var);
std::ostream& operator<<(std::ostream& os, util::sref<inst::type const> type);

#endif /* __STEKIN_INSTANCE_TEST_TEST_COMMON_H__ */
