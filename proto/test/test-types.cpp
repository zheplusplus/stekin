#include <gtest/gtest.h>

#include <test/phony-errors.h>

#include "test-common.h"
#include "../type.h"
#include "../list-types.h"
#include "../func-reference-type.h"

using namespace test;

typedef ProtoTest TypesTest;

TEST_F(TypesTest, Primitives)
{
    EXPECT_EQ(proto::Type::s_void(), proto::Type::s_void());
    EXPECT_EQ(proto::Type::s_bool(), proto::Type::s_bool());
    EXPECT_EQ(proto::Type::s_int(), proto::Type::s_int());
    EXPECT_EQ(proto::Type::s_float(), proto::Type::s_float());

    EXPECT_NE(proto::Type::s_void(), proto::Type::s_float());
    EXPECT_NE(proto::Type::s_void(), proto::Type::s_int());
    EXPECT_NE(proto::Type::s_bool(), proto::Type::s_int());
    EXPECT_NE(proto::Type::s_bool(), proto::Type::s_float());
}

TEST_F(TypesTest, ListTypes)
{
    EXPECT_EQ(proto::ListType::getListType(proto::Type::s_void())
            , proto::ListType::getListType(proto::Type::s_void()));
    EXPECT_EQ(proto::ListType::getListType(proto::Type::s_bool())
            , proto::ListType::getListType(proto::Type::s_bool()));
    EXPECT_EQ(proto::ListType::getListType(proto::Type::s_int())
            , proto::ListType::getListType(proto::Type::s_int()));
    EXPECT_EQ(proto::ListType::getListType(proto::Type::s_float())
            , proto::ListType::getListType(proto::Type::s_float()));

    EXPECT_NE(proto::ListType::getListType(proto::Type::s_void())
            , proto::ListType::getListType(proto::Type::s_float()));
    EXPECT_NE(proto::ListType::getListType(proto::Type::s_void())
            , proto::ListType::getListType(proto::Type::s_int()));
    EXPECT_NE(proto::ListType::getListType(proto::Type::s_bool())
            , proto::ListType::getListType(proto::Type::s_int()));
    EXPECT_NE(proto::ListType::getListType(proto::Type::s_bool())
            , proto::ListType::getListType(proto::Type::s_float()));

    EXPECT_EQ(proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_void()))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_void())));
    EXPECT_EQ(proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_bool()))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_bool())));
    EXPECT_EQ(proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_int()))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_int())));
    EXPECT_EQ(proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_float()))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_float())));

    EXPECT_NE(proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_void()))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_float())));
    EXPECT_NE(proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_void()))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_int())));
    EXPECT_NE(proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_bool()))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_int())));
    EXPECT_NE(proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_bool()))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::s_float())));
}

TEST_F(TypesTest, EmptyListType)
{
    ASSERT_EQ(proto::ListType::getEmptyListType(), proto::ListType::getEmptyListType());
}

TEST_F(TypesTest, ListTypeCompatibility)
{
    EXPECT_EQ(proto::ListType::getEmptyListType()
            , proto::ListType::commonListTypeOrNulIfImcompatible(
                                        proto::ListType::getEmptyListType()
                                      , proto::ListType::getEmptyListType()));
    EXPECT_EQ(proto::ListType::getListType(proto::Type::s_void())
            , proto::ListType::commonListTypeOrNulIfImcompatible(
                                        proto::ListType::getListType(proto::Type::s_void())
                                      , proto::ListType::getEmptyListType()));
    EXPECT_EQ(proto::ListType::getListType(proto::Type::s_int())
            , proto::ListType::commonListTypeOrNulIfImcompatible(
                                        proto::ListType::getEmptyListType()
                                      , proto::ListType::getListType(proto::Type::s_int())));
    EXPECT_EQ(proto::ListType::getListType(proto::Type::s_int())
            , proto::ListType::commonListTypeOrNulIfImcompatible(
                                        proto::ListType::getListType(proto::Type::s_int())
                                      , proto::ListType::getListType(proto::Type::s_int())));
    EXPECT_TRUE(proto::ListType::commonListTypeOrNulIfImcompatible(
                                        proto::ListType::getListType(proto::Type::s_bool())
                                      , proto::ListType::getListType(proto::Type::s_int()))
                .nul());
}

TEST_F(TypesTest, IsListType)
{
    EXPECT_TRUE(proto::ListType::isListType(proto::ListType::getEmptyListType()));
    EXPECT_TRUE(proto::ListType::isListType(proto::ListType::getListType(proto::Type::s_void())));
    EXPECT_TRUE(proto::ListType::isListType(proto::ListType::getListType(proto::Type::s_int())));
    EXPECT_TRUE(proto::ListType::isListType(proto::ListType::getListType(proto::Type::s_float())));
    EXPECT_TRUE(proto::ListType::isListType(proto::ListType::getListType(proto::Type::s_bool())));
    EXPECT_TRUE(proto::ListType::isListType(proto::ListType::getListType(
                                             proto::ListType::getListType(proto::Type::s_bool()))));
    EXPECT_FALSE(proto::ListType::isListType(proto::Type::s_void()));
    EXPECT_FALSE(proto::ListType::isListType(proto::Type::s_int()));
    EXPECT_FALSE(proto::ListType::isListType(proto::Type::s_float()));
    EXPECT_FALSE(proto::ListType::isListType(proto::Type::s_bool()));
}
