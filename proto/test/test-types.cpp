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
    EXPECT_EQ(proto::Type::BIT_VOID, proto::Type::BIT_VOID);
    EXPECT_EQ(proto::Type::BIT_BOOL, proto::Type::BIT_BOOL);
    EXPECT_EQ(proto::Type::BIT_INT, proto::Type::BIT_INT);
    EXPECT_EQ(proto::Type::BIT_FLOAT, proto::Type::BIT_FLOAT);

    EXPECT_NE(proto::Type::BIT_VOID, proto::Type::BIT_FLOAT);
    EXPECT_NE(proto::Type::BIT_VOID, proto::Type::BIT_INT);
    EXPECT_NE(proto::Type::BIT_BOOL, proto::Type::BIT_INT);
    EXPECT_NE(proto::Type::BIT_BOOL, proto::Type::BIT_FLOAT);
}

TEST_F(TypesTest, ListTypes)
{
    EXPECT_EQ(proto::ListType::getListType(proto::Type::BIT_VOID)
            , proto::ListType::getListType(proto::Type::BIT_VOID));
    EXPECT_EQ(proto::ListType::getListType(proto::Type::BIT_BOOL)
            , proto::ListType::getListType(proto::Type::BIT_BOOL));
    EXPECT_EQ(proto::ListType::getListType(proto::Type::BIT_INT)
            , proto::ListType::getListType(proto::Type::BIT_INT));
    EXPECT_EQ(proto::ListType::getListType(proto::Type::BIT_FLOAT)
            , proto::ListType::getListType(proto::Type::BIT_FLOAT));

    EXPECT_NE(proto::ListType::getListType(proto::Type::BIT_VOID)
            , proto::ListType::getListType(proto::Type::BIT_FLOAT));
    EXPECT_NE(proto::ListType::getListType(proto::Type::BIT_VOID)
            , proto::ListType::getListType(proto::Type::BIT_INT));
    EXPECT_NE(proto::ListType::getListType(proto::Type::BIT_BOOL)
            , proto::ListType::getListType(proto::Type::BIT_INT));
    EXPECT_NE(proto::ListType::getListType(proto::Type::BIT_BOOL)
            , proto::ListType::getListType(proto::Type::BIT_FLOAT));

    EXPECT_EQ(proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_VOID))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_VOID)));
    EXPECT_EQ(proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_BOOL))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_BOOL)));
    EXPECT_EQ(proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_INT))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_INT)));
    EXPECT_EQ(proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_FLOAT))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_FLOAT)));

    EXPECT_NE(proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_VOID))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_FLOAT)));
    EXPECT_NE(proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_VOID))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_INT)));
    EXPECT_NE(proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_BOOL))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_INT)));
    EXPECT_NE(proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_BOOL))
            , proto::ListType::getListType(proto::ListType::getListType(proto::Type::BIT_FLOAT)));
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
    EXPECT_EQ(proto::ListType::getListType(proto::Type::BIT_VOID)
            , proto::ListType::commonListTypeOrNulIfImcompatible(
                                        proto::ListType::getListType(proto::Type::BIT_VOID)
                                      , proto::ListType::getEmptyListType()));
    EXPECT_EQ(proto::ListType::getListType(proto::Type::BIT_INT)
            , proto::ListType::commonListTypeOrNulIfImcompatible(
                                        proto::ListType::getEmptyListType()
                                      , proto::ListType::getListType(proto::Type::BIT_INT)));
    EXPECT_EQ(proto::ListType::getListType(proto::Type::BIT_INT)
            , proto::ListType::commonListTypeOrNulIfImcompatible(
                                        proto::ListType::getListType(proto::Type::BIT_INT)
                                      , proto::ListType::getListType(proto::Type::BIT_INT)));
    EXPECT_TRUE(proto::ListType::commonListTypeOrNulIfImcompatible(
                                        proto::ListType::getListType(proto::Type::BIT_BOOL)
                                      , proto::ListType::getListType(proto::Type::BIT_INT))
                .nul());
}

TEST_F(TypesTest, IsListType)
{
    EXPECT_TRUE(proto::ListType::isListType(proto::ListType::getEmptyListType()));
    EXPECT_TRUE(proto::ListType::isListType(proto::ListType::getListType(proto::Type::BIT_VOID)));
    EXPECT_TRUE(proto::ListType::isListType(proto::ListType::getListType(proto::Type::BIT_INT)));
    EXPECT_TRUE(proto::ListType::isListType(proto::ListType::getListType(proto::Type::BIT_FLOAT)));
    EXPECT_TRUE(proto::ListType::isListType(proto::ListType::getListType(proto::Type::BIT_BOOL)));
    EXPECT_TRUE(proto::ListType::isListType(proto::ListType::getListType(
                                             proto::ListType::getListType(proto::Type::BIT_BOOL))));
    EXPECT_FALSE(proto::ListType::isListType(proto::Type::BIT_VOID));
    EXPECT_FALSE(proto::ListType::isListType(proto::Type::BIT_INT));
    EXPECT_FALSE(proto::ListType::isListType(proto::Type::BIT_FLOAT));
    EXPECT_FALSE(proto::ListType::isListType(proto::Type::BIT_BOOL));
}
