#ifndef __STEKIN_INSTANCE_TEST_TEST_COMMON_H__
#define __STEKIN_INSTANCE_TEST_TEST_COMMON_H__

#include <gtest/gtest.h>

#include "../type.h"
#include "../node-base.h"

namespace test {

    extern inst::BuiltInPrimitive const WORD;
    extern inst::BuiltInPrimitive const DWORD;
    extern inst::BuiltInPrimitive const HALFWORD;

    struct CondTypeExpr
        : public inst::Expression
    {
        util::sref<inst::Type const> typeof() const
        {
            return inst::Type::BIT_BOOL;
        }

        void write() const {}
    };

    struct VoidTypeExpr
        : public inst::Expression
    {
        util::sref<inst::Type const> typeof() const
        {
            return inst::Type::BIT_VOID;
        }

        void write() const {}
    };

    struct BadTypeExpr
        : public inst::Expression
    {
        util::sref<inst::Type const> typeof() const
        {
            return inst::Type::BAD_TYPE;
        }

        void write() const {}
    };

    struct InstanceTest
        : public testing::Test
    {
        void SetUp();
    };

}

std::ostream& operator<<(std::ostream& os, inst::Variable const& var);
std::ostream& operator<<(std::ostream& os, util::sref<inst::Type const> type);

#endif /* __STEKIN_INSTANCE_TEST_TEST_COMMON_H__ */
