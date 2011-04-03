#include <gtest/gtest.h>

#include "test-common.h"
#include "../clause-builder.h"
#include "../function.h"
#include "../stmt-nodes.h"
#include "../expr-nodes.h"
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
    stack0.next_stmt(0, std::move(
                util::mkptr(new grammar::var_def(item_pos, "ruby", std::move(
                            util::mkptr(new grammar::reference(item_pos, "emerald")))))));
    stack0.next_stmt(0, std::move(
                util::mkptr(new grammar::var_def(item_pos, "topiz", std::move(
                            util::mkptr(new grammar::reference(item_pos, "ruby")))))));
    stack0.next_func(0, std::move(util::mkptr(new grammar::function(item_pos
                                                                  , "skull"
                                                                  , std::vector<std::string>({ "chipped" })
                                                                  , std::move(grammar::block())))));
    ASSERT_FALSE(error::has_error());
    grammar::block block0(std::move(stack0.pack_all()));
    block0.compile(std::move(util::mkmptr(new flchk::filter)));

    data_tree::expect_one()
        (item_pos, FUNC_DEF, "skull")
            (item_pos, PARAMETER, "chipped")

            (item_pos, REFERENCE, "emerald")
        (item_pos, VAR_DEF, "ruby")
            (item_pos, REFERENCE, "ruby")
        (item_pos, VAR_DEF, "topiz")
    ;

    misc::pos_type err_pos0(101);
    misc::pos_type err_pos1(102);
    ASSERT_FALSE(error::has_error());
    stack0.next_stmt(1, std::move(
                util::mkptr(new grammar::arithmetics(err_pos0, std::move(
                            util::mkptr(new grammar::int_literal(item_pos, "20110119")))))));
    stack0.next_stmt(1, std::move(
                util::mkptr(new grammar::arithmetics(item_pos, std::move(
                            util::mkptr(new grammar::float_literal(item_pos, "19.55")))))));
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
    stack0.next_stmt(0, std::move(
                util::mkptr(new grammar::arithmetics(item_pos, std::move(
                            util::mkptr(new grammar::reference(item_pos, "eaglehorn")))))));
    stack0.add(0, std::move(util::mkmptr(new grammar::function_acceptor(acc_pos
                                                                      , "witherstring"
                                                                      , std::vector<std::string>()))));
    stack0.next_stmt(1, std::move(
                util::mkptr(new grammar::var_def(item_pos, "cedar_bow", std::move(
                            util::mkptr(new grammar::reference(item_pos, "kuko_shakaku"))))))); 
    stack0.next_stmt(1, std::move(
                util::mkptr(new grammar::arithmetics(item_pos, std::move(
                            util::mkptr(new grammar::reference(item_pos, "buriza_do_kyanon"))))))); 
    ASSERT_FALSE(error::has_error());
    grammar::block block0(std::move(stack0.pack_all()));
    block0.compile(std::move(util::mkmptr(new flchk::filter)));

    data_tree::expect_one()
                (item_pos, REFERENCE, "kuko_shakaku")
            (item_pos, VAR_DEF, "cedar_bow")
                (item_pos, REFERENCE, "buriza_do_kyanon")
            (item_pos, ARITHMETICS)
        (acc_pos, FUNC_DEF, "witherstring")

            (item_pos, REFERENCE, "eaglehorn")
        (item_pos, ARITHMETICS)
    ;

    grammar::acceptor_stack stack1;
    stack1.add(0, std::move(util::mkmptr(new grammar::function_acceptor(acc_pos
                                                                      , "witherstring"
                                                                      , std::vector<std::string>()))));
    stack1.next_stmt(1, std::move(
                util::mkptr(new grammar::var_def(item_pos, "cedar_bow", std::move(
                            util::mkptr(new grammar::reference(item_pos, "kuko_shakaku"))))))); 
    stack1.next_stmt(0, std::move(
                util::mkptr(new grammar::arithmetics(item_pos, std::move(
                            util::mkptr(new grammar::reference(item_pos, "eaglehorn")))))));
    stack1.next_stmt(1, std::move(
                util::mkptr(new grammar::arithmetics(item_pos, std::move(
                            util::mkptr(new grammar::reference(item_pos, "buriza_do_kyanon"))))))); 
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
    stack0.next_stmt(0, std::move(
                util::mkptr(new grammar::arithmetics(item_pos, std::move(
                            util::mkptr(new grammar::reference(item_pos, "roguesbow")))))));
    stack0.add(0, std::move(util::mkmptr(new grammar::if_acceptor(
                          acc_pos
                        , util::mkptr(new grammar::reference(item_pos, "stormstrike"))))));
    stack0.next_stmt(1, std::move(
                util::mkptr(new grammar::arithmetics(item_pos, std::move(
                            util::mkptr(new grammar::reference(item_pos, "witchwild_string"))))))); 
    stack0.match_else(0, else_pos);
    stack0.next_stmt(1, std::move(
                util::mkptr(new grammar::arithmetics(item_pos, std::move(
                            util::mkptr(new grammar::reference(item_pos, "magewrath"))))))); 
    ASSERT_FALSE(error::has_error());
    grammar::block block0(std::move(stack0.pack_all()));
    block0.compile(std::move(util::mkmptr(new flchk::filter)));

    data_tree::expect_one()
            (item_pos, REFERENCE, "roguesbow")
        (item_pos, ARITHMETICS)
                (item_pos, REFERENCE, "magewrath")
            (item_pos, ARITHMETICS)
                (item_pos, REFERENCE, "witchwild_string")
            (item_pos, ARITHMETICS)
        (item_pos, REFERENCE, "stormstrike")
        (acc_pos, BRANCH)
    ;

    grammar::acceptor_stack stack1;
    stack1.next_stmt(0, std::move(
                util::mkptr(new grammar::arithmetics(item_pos, std::move(
                            util::mkptr(new grammar::reference(item_pos, "roguesbow")))))));
    stack1.add(0, std::move(util::mkmptr(new grammar::if_acceptor(
                          acc_pos
                        , util::mkptr(new grammar::reference(item_pos, "stormstrike"))))));
    stack1.next_stmt(1, std::move(
                util::mkptr(new grammar::arithmetics(item_pos, std::move(
                            util::mkptr(new grammar::reference(item_pos, "witchwild_string"))))))); 
    stack1.match_else(2, else_pos);
    stack1.next_stmt(1, std::move(
                util::mkptr(new grammar::arithmetics(item_pos, std::move(
                            util::mkptr(new grammar::reference(item_pos, "magewrath"))))))); 
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
    builder0.add_if(0, std::move(util::mkptr(new grammar::bool_literal(item_pos0, true))));
        builder0.add_var_def(1, "wind_force", std::move(util::mkptr(new grammar::int_literal(item_pos1
                                                                                           , "13571"))));
        builder0.add_ifnot(1, std::move(util::mkptr(new grammar::reference(item_pos2, "raven_claw"))));
            builder0.add_return(2, std::move(util::mkptr(new grammar::float_literal(item_pos0, "0.000123"))));
        builder0.add_return_nothing(1, item_pos1);
    builder0.add_else(0, item_pos2);
        builder0.add_ifnot(1, std::move(util::mkptr(new grammar::reference(item_pos2, "cliffkiller"))));
            builder0.add_ifnot(2, std::move(util::mkptr(new grammar::reference(item_pos0, "skystrike"))));
    builder0.add_function(0, item_pos1, "goldenstrike_arch", std::vector<std::string>({ "amn", "tir" }));
        builder0.add_arith(1, std::move(util::mkptr(new grammar::reference(item_pos1, "widowmaker"))));

    builder0.build_and_clear();
    ASSERT_FALSE(error::has_error());

    data_tree::expect_one()
                (item_pos1, REFERENCE, "widowmaker")
            (item_pos1, ARITHMETICS)
        (item_pos1, FUNC_DEF, "goldenstrike_arch")
            (item_pos1, PARAMETER, "amn")
            (item_pos1, PARAMETER, "tir")

                (item_pos0, REFERENCE, "skystrike")
                (item_pos0, BRANCH_ALTER_ONLY)
            (item_pos2, REFERENCE, "cliffkiller")
            (item_pos2, BRANCH_ALTER_ONLY)

                (item_pos1, INTEGER, "13571")
            (item_pos1, VAR_DEF_FILTERED, "wind_force")

                    (item_pos0, FLOATING, "0.000123")
                (item_pos0, RETURN)
            (item_pos2, REFERENCE, "raven_claw")
            (item_pos2, BRANCH_ALTER_ONLY)

            (item_pos1, RETURN_NOTHING)
        (item_pos0, BOOLEAN, "true")
        (item_pos0, BRANCH)
    ;

    grammar::clause_builder builder1;
    builder1.add_if(0, std::move(util::mkptr(new grammar::bool_literal(item_pos0, true))));
    builder1.add_var_def(0, "wind_force", std::move(util::mkptr(new grammar::int_literal(item_pos1, "13571"))));
    builder1.add_else(0, item_pos2);
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_else_not_matches().size());
    ASSERT_EQ(item_pos2, get_else_not_matches()[0].pos);
}
