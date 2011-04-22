#include <gtest/gtest.h>

#include "test-common.h"
#include "../expr-nodes.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"

using namespace test;

typedef instance_test ExprNodesTest;

TEST_F(ExprNodesTest, ConjunctionConditionTypeCheck)
{
    misc::position pos(1);
    inst::Conjunction(pos
                    , std::move(util::mkptr(new cond_type_expr))
                    , std::move(util::mkptr(new cond_type_expr)));
    ASSERT_FALSE(error::hasError());
    inst::Conjunction(pos
                    , std::move(util::mkptr(new cond_type_expr))
                    , std::move(util::mkptr(new bad_type_expr)));
    ASSERT_FALSE(error::hasError());
    inst::Conjunction(pos
                    , std::move(util::mkptr(new bad_type_expr))
                    , std::move(util::mkptr(new cond_type_expr)));
    ASSERT_FALSE(error::hasError());
    inst::Conjunction(pos
                    , std::move(util::mkptr(new bad_type_expr))
                    , std::move(util::mkptr(new bad_type_expr)));
    ASSERT_FALSE(error::hasError());

    misc::position pos_err0(10);
    inst::Conjunction(pos_err0
                    , std::move(util::mkptr(new void_type_expr))
                    , std::move(util::mkptr(new cond_type_expr)));
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, getCondNotBools().size());

    misc::position pos_err1(11);
    inst::Conjunction(pos_err1
                    , std::move(util::mkptr(new cond_type_expr))
                    , std::move(util::mkptr(new void_type_expr)));
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(2, getCondNotBools().size());

    misc::position pos_err2(12);
    inst::Conjunction(pos_err2
                    , std::move(util::mkptr(new void_type_expr))
                    , std::move(util::mkptr(new void_type_expr)));
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(4, getCondNotBools().size());

    ASSERT_EQ(pos_err0, getCondNotBools()[0].pos);
    ASSERT_EQ("void", getCondNotBools()[0].type_name);

    ASSERT_EQ(pos_err1, getCondNotBools()[1].pos);
    ASSERT_EQ("void", getCondNotBools()[1].type_name);

    ASSERT_EQ(pos_err2, getCondNotBools()[2].pos);
    ASSERT_EQ("void", getCondNotBools()[2].type_name);

    ASSERT_EQ(pos_err2, getCondNotBools()[3].pos);
    ASSERT_EQ("void", getCondNotBools()[3].type_name);
}

TEST_F(ExprNodesTest, DisjunctionConditionTypeCheck)
{
    misc::position pos(2);
    inst::Disjunction(pos
                    , std::move(util::mkptr(new cond_type_expr))
                    , std::move(util::mkptr(new cond_type_expr)));
    ASSERT_FALSE(error::hasError());
    inst::Disjunction(pos
                    , std::move(util::mkptr(new cond_type_expr))
                    , std::move(util::mkptr(new bad_type_expr)));
    ASSERT_FALSE(error::hasError());
    inst::Disjunction(pos
                    , std::move(util::mkptr(new bad_type_expr))
                    , std::move(util::mkptr(new cond_type_expr)));
    ASSERT_FALSE(error::hasError());
    inst::Disjunction(pos
                    , std::move(util::mkptr(new bad_type_expr))
                    , std::move(util::mkptr(new bad_type_expr)));
    ASSERT_FALSE(error::hasError());

    misc::position pos_err0(20);
    inst::Disjunction(pos_err0
                    , std::move(util::mkptr(new void_type_expr))
                    , std::move(util::mkptr(new cond_type_expr)));
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, getCondNotBools().size());

    misc::position pos_err1(21);
    inst::Disjunction(pos_err1
                    , std::move(util::mkptr(new cond_type_expr))
                    , std::move(util::mkptr(new void_type_expr)));
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(2, getCondNotBools().size());

    misc::position pos_err2(22);
    inst::Disjunction(pos_err2
                    , std::move(util::mkptr(new void_type_expr))
                    , std::move(util::mkptr(new void_type_expr)));
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(4, getCondNotBools().size());

    ASSERT_EQ(pos_err0, getCondNotBools()[0].pos);
    ASSERT_EQ("void", getCondNotBools()[0].type_name);

    ASSERT_EQ(pos_err1, getCondNotBools()[1].pos);
    ASSERT_EQ("void", getCondNotBools()[1].type_name);

    ASSERT_EQ(pos_err2, getCondNotBools()[2].pos);
    ASSERT_EQ("void", getCondNotBools()[2].type_name);

    ASSERT_EQ(pos_err2, getCondNotBools()[3].pos);
    ASSERT_EQ("void", getCondNotBools()[3].type_name);
}

TEST_F(ExprNodesTest, NegationConditionTypeCheck)
{
    misc::position pos(3);
    inst::Negation(pos, std::move(util::mkptr(new cond_type_expr)));
    ASSERT_FALSE(error::hasError());
    inst::Negation(pos, std::move(util::mkptr(new bad_type_expr)));
    ASSERT_FALSE(error::hasError());

    inst::Negation(pos, std::move(util::mkptr(new void_type_expr)));
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, getCondNotBools().size());
    ASSERT_EQ(pos, getCondNotBools()[0].pos);
    ASSERT_EQ("void", getCondNotBools()[0].type_name);
}
