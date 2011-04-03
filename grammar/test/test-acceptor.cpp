#include <gtest/gtest.h>

#include "test-common.h"
#include "../acceptor.h"
#include "../function.h"
#include "../stmt-nodes.h"
#include "../expr-nodes.h"
#include "../../flowcheck/node-base.h"
#include "../../flowcheck/filter.h"
#include "../../flowcheck/function.h"
#include "../../proto/node-base.h"
#include "../../test/phony-errors.h"

using namespace test;

struct test_acceptor
    : public grammar::acceptor
{
    test_acceptor()
        : grammar::acceptor(misc::pos_type(0))
    {}

    void accept_stmt(util::sptr<grammar::stmt_base const> s)
    {
        stmt = std::move(s);
    }

    void accept_func(util::sptr<grammar::function const> f)
    {
        func = std::move(f);
    }

    void compile()
    {
        grammar::block block;
        if (bool(stmt)) {
            block.add_stmt(std::move(stmt));
        }
        if (bool(func)) {
            block.add_func(std::move(func));
        }
        block.compile(std::move(util::mkmptr(new flchk::filter)));
    }

    util::sptr<grammar::stmt_base const> stmt;
    util::sptr<grammar::function const> func;

    void deliver_to(util::sref<grammar::acceptor>) {}
};

typedef grammar_test AcceptorTest;

TEST_F(AcceptorTest, IfAcceptor)
{
    misc::pos_type pos(1);
    misc::pos_type pos_head(100);
    misc::pos_type pos_else(101);
    test_acceptor receiver;

    grammar::if_acceptor acceptor_a(pos_head, std::move(util::mkptr(new grammar::int_literal(pos_head, "0"))));
    acceptor_a.accept_stmt(std::move(util::mkptr(new grammar::func_ret_nothing(pos))));

    acceptor_a.accept_else(pos_else);
    ASSERT_FALSE(error::has_error());
    acceptor_a.accept_stmt(std::move(
                util::mkptr(new grammar::var_def(pos, "Hyperion", std::move(
                            util::mkptr(new grammar::reference(pos, "Raynor")))))));

    acceptor_a.deliver_to(util::mkref(receiver));
    ASSERT_TRUE(bool(receiver.stmt));
    ASSERT_FALSE(bool(receiver.func));
    receiver.compile();

    grammar::if_acceptor acceptor_b(pos_head, std::move(util::mkptr(new grammar::int_literal(pos_head, "1"))));
    acceptor_b.accept_stmt(std::move(
                util::mkptr(new grammar::func_ret(pos, std::move(
                            util::mkptr(new grammar::reference(pos, "Karrigan")))))));

    acceptor_b.deliver_to(util::mkref(receiver));
    ASSERT_TRUE(bool(receiver.stmt));
    ASSERT_FALSE(bool(receiver.func));
    receiver.compile();

    grammar::if_acceptor acceptor_c(pos_head, std::move(util::mkptr(new grammar::int_literal(pos_head, "2"))));
    acceptor_c.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::bool_literal(pos, false)))))));
    acceptor_c.accept_else(pos_else);
    ASSERT_FALSE(error::has_error());

    acceptor_c.deliver_to(util::mkref(receiver));
    ASSERT_TRUE(bool(receiver.stmt));
    ASSERT_FALSE(bool(receiver.func));
    receiver.compile();

    grammar::if_acceptor acceptor_d(pos_head, std::move(util::mkptr(new grammar::int_literal(pos_head, "3"))));
    acceptor_d.accept_else(pos_else);
    ASSERT_FALSE(error::has_error());
    acceptor_d.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::float_literal(pos, "20.54")))))));

    acceptor_d.deliver_to(util::mkref(receiver));
    ASSERT_TRUE(bool(receiver.stmt));
    ASSERT_FALSE(bool(receiver.func));
    receiver.compile();

    data_tree::expect_one()
                (pos, REFERENCE, "Raynor")
            (pos, VAR_DEF_FILTERED, "Hyperion")
            (pos, RETURN_NOTHING)
        (pos_head, INTEGER, "0")
        (pos_head, BRANCH)

                (pos, REFERENCE, "Karrigan")
            (pos, RETURN)
        (pos_head, INTEGER, "1")
        (pos_head, BRANCH_CONSQ_ONLY)

                (pos, BOOLEAN, "false")
            (pos, ARITHMETICS)
        (pos_head, INTEGER, "2")
        (pos_head, BRANCH)

                (pos, FLOATING, "20.54")
            (pos, ARITHMETICS)
        (pos_head, INTEGER, "3")
        (pos_head, BRANCH)
    ;
    ASSERT_FALSE(error::has_error());
}

TEST_F(AcceptorTest, IfAcceptorError)
{
    misc::pos_type pos(2);
    misc::pos_type pos_head(200);
    misc::pos_type pos_else(201);
    grammar::if_acceptor acceptor_a(pos_head, std::move(util::mkptr(new grammar::int_literal(pos_head, "0"))));
    acceptor_a.accept_else(pos);
    ASSERT_FALSE(error::has_error());
    acceptor_a.accept_else(pos_else);
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_if_matcheds().size());
    ASSERT_EQ(pos, get_if_matcheds()[0].prev_pos);
    ASSERT_EQ(pos_else, get_if_matcheds()[0].this_pos);
}

TEST_F(AcceptorTest, IfNotAcceptor)
{
    misc::pos_type pos(3);
    test_acceptor receiver;

    grammar::ifnot_acceptor ifnot_acc0(pos, std::move(util::mkptr(new grammar::bool_literal(pos, false))));
    ifnot_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::var_def(pos, "SCV", std::move(
                            util::mkptr(new grammar::int_literal(pos, "60")))))));
    ifnot_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::reference(pos, "Marine")))))));

    ifnot_acc0.deliver_to(util::mkref(receiver));
    ASSERT_TRUE(bool(receiver.stmt));
    ASSERT_FALSE(bool(receiver.func));
    receiver.compile();

    data_tree::expect_one()
                (pos, INTEGER, "60")
            (pos, VAR_DEF_FILTERED, "SCV")
                (pos, REFERENCE, "Marine")
            (pos, ARITHMETICS)
        (pos, BOOLEAN, "false")
        (pos, BRANCH_ALTER_ONLY)
    ;
    ASSERT_FALSE(error::has_error());

    misc::pos_type pos_else(20);
    grammar::ifnot_acceptor ifnot_acc1(pos, std::move(util::mkptr(new grammar::bool_literal(pos, true))));
    ifnot_acc1.accept_else(pos_else);
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_else_not_matches().size());
    ASSERT_EQ(pos_else, get_else_not_matches()[0].pos);
}

TEST_F(AcceptorTest, FuncAcceptor)
{
    misc::pos_type pos(5);
    test_acceptor receiver;

    grammar::function_acceptor func_acc0(pos, "func1", std::vector<std::string>({ "Duke", "Duran" }));
    func_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::float_literal(pos, "21.37")))))));
    func_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::var_def(pos, "SonOfKorhal", std::move(
                            util::mkptr(new grammar::int_literal(pos, "20110116")))))));

    func_acc0.deliver_to(util::mkref(receiver));
    ASSERT_FALSE(bool(receiver.stmt));
    ASSERT_TRUE(bool(receiver.func));
    receiver.compile();

    data_tree::expect_one()
                (pos, FLOATING, "21.37")
            (pos, ARITHMETICS)
                (pos, INTEGER, "20110116")
            (pos, VAR_DEF, "SonOfKorhal")
        (pos, FUNC_DEF, "func1")
            (pos, PARAMETER, "Duke")
            (pos, PARAMETER, "Duran")
    ;
    ASSERT_FALSE(error::has_error());

    misc::pos_type pos_else(10);
    grammar::function_acceptor func_acc1(pos, "func2", std::vector<std::string>({ "Mengsk" }));
    func_acc1.accept_else(pos_else);
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_else_not_matches().size());
    ASSERT_EQ(pos_else, get_else_not_matches()[0].pos);
}

TEST_F(AcceptorTest, FuncAccNested)
{
    misc::pos_type pos(5);
    test_acceptor receiver;

    grammar::function_acceptor func_acc0(pos, "funca", std::vector<std::string>({ "firebat", "ghost" }));
    func_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::float_literal(pos, "22.15")))))));
    func_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::var_def(pos, "medic", std::move(
                            util::mkptr(new grammar::reference(pos, "wraith")))))));

    grammar::function_acceptor func_acc1(pos, "funca", std::vector<std::string>({ "vulture" }));
    func_acc1.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::reference(pos, "goliath")))))));

    func_acc1.deliver_to(util::mkref(func_acc0));
    func_acc0.deliver_to(util::mkref(receiver));
    ASSERT_FALSE(bool(receiver.stmt));
    ASSERT_TRUE(bool(receiver.func));
    receiver.compile();

    data_tree::expect_one()
                    (pos, REFERENCE, "goliath")
                (pos, ARITHMETICS)
            (pos, FUNC_DEF, "funca")
                (pos, PARAMETER, "vulture")

                (pos, FLOATING, "22.15")
            (pos, ARITHMETICS)
                (pos, REFERENCE, "wraith")
            (pos, VAR_DEF, "medic")
        (pos, FUNC_DEF, "funca")
            (pos, PARAMETER, "firebat")
            (pos, PARAMETER, "ghost")
    ;
    ASSERT_FALSE(error::has_error());
}
