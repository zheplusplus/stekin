#include <gtest/gtest.h>

#include "test-common.h"
#include "../clause-builder.h"
#include "../function.h"
#include "../stmt-nodes.h"
#include "../../flowcheck/node-base.h"
#include "../../flowcheck/filter.h"
#include "../../flowcheck/function.h"
#include "../../proto/node-base.h"
#include "../../test/phony-errors.h"

using namespace test;

typedef grammar_test ClauseBuilderTest;

TEST_F(ClauseBuilderTest, AcceptorStackNext)
{
    misc::pos_type item_pos(1);
    misc::pos_type acc_pos(100);

    grammar::acceptor_stack stack0;
    stack0.next_stmt(0, std::move(util::mkmptr(new grammar::var_def(item_pos, "ruby", std::move(mkexpr())))));
    stack0.next_stmt(0, std::move(util::mkmptr(new grammar::var_def(item_pos, "topiz", std::move(mkexpr())))));
    stack0.next_func(0, std::move(util::mkptr(new grammar::function(item_pos
                                                                  , "skull"
                                                                  , std::vector<std::string>({ "chipped" })
                                                                  , std::move(grammar::block())))));
    ASSERT_FALSE(error::has_error());
    grammar::block block0(std::move(stack0.pack_all()));
    block0.compile(std::move(util::mkmptr(new flchk::filter)));

    data_tree::expect_one()
        (item_pos, FUNC_DEF, "skull")

        (item_pos, VAR_DEF, "ruby")
        (item_pos, VAR_DEF, "topiz")
    ;

    misc::pos_type err_pos0(101);
    misc::pos_type err_pos1(102);
    ASSERT_FALSE(error::has_error());
    stack0.next_stmt(1, std::move(util::mkmptr(new grammar::arithmetics(err_pos0, std::move(mkexpr())))));
    stack0.next_stmt(1, std::move(util::mkmptr(new grammar::arithmetics(item_pos, std::move(mkexpr())))));
    stack0.next_func(4, std::move(util::mkptr(new grammar::function(err_pos1
                                                                  , "ith"
                                                                  , std::vector<std::string>({ "el", "eth" })
                                                                  , std::move(grammar::block())))));
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(2, get_excess_inds().size());
    ASSERT_EQ(err_pos0, get_excess_inds()[0].pos);
    ASSERT_EQ(err_pos1, get_excess_inds()[1].pos);
}

TEST_F(ClauseBuilderTest, AcceptorStackAdd)
{
    misc::pos_type item_pos(2);
    misc::pos_type acc_pos(100);

    grammar::acceptor_stack stack0;
    stack0.next_stmt(0, std::move(util::mkmptr(new grammar::arithmetics(item_pos, std::move(mkexpr())))));
    stack0.add(0, std::move(util::mkmptr(new grammar::function_acceptor(acc_pos
                                                                      , "witherstring"
                                                                      , std::vector<std::string>()))));
    stack0.next_stmt(1, std::move(util::mkmptr(new grammar::var_def(item_pos
                                                                  , "cedar_bow"
                                                                  , std::move(mkexpr())))));
    stack0.next_stmt(1, std::move(util::mkmptr(new grammar::arithmetics(item_pos, std::move(mkexpr())))));
    ASSERT_FALSE(error::has_error());
    grammar::block block0(std::move(stack0.pack_all()));
    block0.compile(std::move(util::mkmptr(new flchk::filter)));

    data_tree::expect_one()
        (acc_pos, FUNC_DEF, "witherstring")
            (item_pos, VAR_DEF, "cedar_bow")
            (item_pos, ARITHMETICS)

        (item_pos, ARITHMETICS)
    ;

    grammar::acceptor_stack stack1;
    stack1.add(0, std::move(util::mkmptr(new grammar::function_acceptor(acc_pos
                                                                      , "witherstring"
                                                                      , std::vector<std::string>()))));
    stack1.next_stmt(1, std::move(util::mkmptr(new grammar::var_def(item_pos
                                                                  , "cedar_bow"
                                                                  , std::move(mkexpr())))));
    stack1.next_stmt(0, std::move(util::mkmptr(new grammar::arithmetics(item_pos, std::move(mkexpr())))));
    stack1.next_stmt(1, std::move(util::mkmptr(new grammar::arithmetics(item_pos, std::move(mkexpr())))));
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_excess_inds().size());
    ASSERT_EQ(item_pos, get_excess_inds()[0].pos);
}

TEST_F(ClauseBuilderTest, AcceptorStackMatchElse)
{
    misc::pos_type item_pos(3);
    misc::pos_type acc_pos(210);
    misc::pos_type else_pos(211);

    grammar::acceptor_stack stack0;
    stack0.next_stmt(0, std::move(util::mkmptr(new grammar::arithmetics(item_pos, std::move(mkexpr())))));
    stack0.add(0, std::move(util::mkmptr(new grammar::if_acceptor(acc_pos, mkexpr()))));
    stack0.next_stmt(1, std::move(util::mkmptr(new grammar::arithmetics(item_pos, std::move(mkexpr())))));
    stack0.match_else(0, else_pos);
    stack0.next_stmt(1, std::move(util::mkmptr(new grammar::arithmetics(item_pos, std::move(mkexpr())))));
    ASSERT_FALSE(error::has_error());
    grammar::block block0(std::move(stack0.pack_all()));
    block0.compile(std::move(util::mkmptr(new flchk::filter)));

    data_tree::expect_one()
        (item_pos, ARITHMETICS)
            (item_pos, ARITHMETICS)
            (item_pos, ARITHMETICS)
        (acc_pos, BRANCH)
    ;

    grammar::acceptor_stack stack1;
    stack1.next_stmt(0, std::move(util::mkmptr(new grammar::arithmetics(item_pos, std::move(mkexpr())))));
    stack1.add(0, std::move(util::mkmptr(new grammar::if_acceptor(acc_pos, std::move(mkexpr())))));
    stack1.next_stmt(1, std::move(util::mkmptr(new grammar::arithmetics(item_pos, std::move(mkexpr())))));
    stack1.match_else(2, else_pos);
    stack1.next_stmt(1, std::move(util::mkmptr(new grammar::arithmetics(item_pos, std::move(mkexpr())))));
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_else_not_matches().size());
    ASSERT_EQ(else_pos, get_else_not_matches()[0].pos);
}

TEST_F(ClauseBuilderTest, ClauseBuilder)
{
    misc::pos_type item_pos0(4);
    misc::pos_type item_pos1(5);
    misc::pos_type item_pos2(6);

    grammar::clause_builder builder0;
    builder0.add_if(0, std::move(mkexpr()));
    builder0.add_var_def(1, "wind_force", std::move(mkexpr()));
    builder0.add_if(1, std::move(mkexpr()));
    builder0.add_return(2, std::move(mkexpr()));
    builder0.add_return_nothing(1, item_pos1);
    builder0.add_else(0, item_pos2);
    builder0.add_ifnot(1, std::move(mkexpr()));
    builder0.add_if(2, std::move(mkexpr()));
    builder0.add_function(0, item_pos1, "goldenstrike_arch", std::vector<std::string>({ "amn", "tir" }));
    builder0.add_arith(1, std::move(mkexpr()));

    builder0.build_and_clear();
    ASSERT_FALSE(error::has_error());

    data_tree::expect_one()
        (item_pos1, FUNC_DEF, "goldenstrike_arch")
            (item_pos1, PARAMETER, "amn")
            (item_pos1, PARAMETER, "tir")

            (item_pos1, ARITHMETICS)
            (item_pos1, VAR_DEF, "wind_force")

                (item_pos0, RETURN)
            (item_pos2, BRANCH)

            (item_pos1, RETURN_NOTHING)
                (item_pos0, BRANCH)
            (item_pos2, BRANCH)
        (item_pos0, BRANCH)
    ;

    grammar::clause_builder builder1;
    builder1.add_if(0, std::move(mkexpr()));
    builder1.add_var_def(0, "wind_force", std::move(mkexpr()));
    builder1.add_else(0, item_pos2);
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_else_not_matches().size());
    ASSERT_EQ(item_pos2, get_else_not_matches()[0].pos);
}
