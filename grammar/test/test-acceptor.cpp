#include <gtest/gtest.h>

#include "test-common.h"
#include "../acceptor.h"
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

    void accept_func(util::sptr<grammar::func_def const> f)
    {
        func = std::move(f);
    }

    void compile(util::sref<proto::scope> scope)
    {
        grammar::block block;
        if (bool(stmt)) {
            block.add_stmt(std::move(stmt));
        }
        if (bool(func)) {
            block.add_func(std::move(func));
        }
        block.compile(scope);
    }

    util::sptr<grammar::stmt_base const> stmt;
    util::sptr<grammar::func_def const> func;

    void deliver_to(util::sref<grammar::acceptor>) {}
};

typedef grammar_test AcceptorTest;

TEST_F(AcceptorTest, IfAcceptor)
{
    misc::pos_type pos(1);
    misc::pos_type pos_head(100);
    misc::pos_type pos_else(101);
    test_acceptor receiver;
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));

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
    receiver.compile(*scope);

    grammar::if_acceptor acceptor_b(pos_head, std::move(util::mkptr(new grammar::int_literal(pos_head, "1"))));
    acceptor_b.accept_stmt(std::move(
                util::mkptr(new grammar::func_ret(pos, std::move(
                            util::mkptr(new grammar::reference(pos, "Karrigan")))))));

    acceptor_b.deliver_to(util::mkref(receiver));
    ASSERT_TRUE(bool(receiver.stmt));
    ASSERT_FALSE(bool(receiver.func));
    receiver.compile(*scope);

    grammar::if_acceptor acceptor_c(pos_head, std::move(util::mkptr(new grammar::int_literal(pos_head, "2"))));
    acceptor_c.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::bool_literal(pos, false)))))));
    acceptor_c.accept_else(pos_else);
    ASSERT_FALSE(error::has_error());

    acceptor_c.deliver_to(util::mkref(receiver));
    ASSERT_TRUE(bool(receiver.stmt));
    ASSERT_FALSE(bool(receiver.func));
    receiver.compile(*scope);

    grammar::if_acceptor acceptor_d(pos_head, std::move(util::mkptr(new grammar::int_literal(pos_head, "3"))));
    acceptor_d.accept_else(pos_else);
    ASSERT_FALSE(error::has_error());
    acceptor_d.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::float_literal(pos, "20.54")))))));

    acceptor_d.deliver_to(util::mkref(receiver));
    ASSERT_TRUE(bool(receiver.stmt));
    ASSERT_FALSE(bool(receiver.func));
    receiver.compile(*scope);

    data_tree::expect_one()
    (SCOPE)
        (SCOPE)
            (pos, RETURN_NOTHING)
        (SCOPE)
                (pos, REFERENCE, "Raynor")
            (pos, VAR_DEF, "Hyperion")
        (pos_head, INTEGER, "0")
        (pos_head, BRANCH)

        (SCOPE)
                (pos, REFERENCE, "Karrigan")
            (pos, RETURN)
        (SCOPE)
        (pos_head, INTEGER, "1")
        (pos_head, BRANCH)

        (SCOPE)
                (pos, BOOLEAN, "false")
            (pos, ARITHMETICS)
        (SCOPE)
        (pos_head, INTEGER, "2")
        (pos_head, BRANCH)

        (SCOPE)
        (SCOPE)
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

    clear_err();
    misc::pos_type pos_func(210);
    grammar::if_acceptor acceptor_b(pos_head, std::move(util::mkptr(new grammar::int_literal(pos_head, "1"))));
    acceptor_b.accept_else(pos);
    ASSERT_FALSE(error::has_error());
    acceptor_b.accept_func(std::move(util::mkptr(new grammar::func_def(pos_func
                                                                     , "_null_"
                                                                     , std::vector<std::string>({ "Nexus" })
                                                                     , std::move(grammar::block())))));
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_forbidden_funcs().size());
    ASSERT_EQ(pos_func, get_forbidden_funcs()[0].pos);
    ASSERT_EQ("_null_", get_forbidden_funcs()[0].name);
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
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));
    ASSERT_TRUE(bool(receiver.stmt));
    ASSERT_FALSE(bool(receiver.func));
    receiver.compile(*scope);

    data_tree::expect_one()
    (SCOPE)
        (SCOPE)
        (SCOPE)
                (pos, INTEGER, "60")
            (pos, VAR_DEF, "SCV")
                (pos, REFERENCE, "Marine")
            (pos, ARITHMETICS)
        (pos, BOOLEAN, "false")
        (pos, BRANCH)
    ;
    ASSERT_FALSE(error::has_error());

    misc::pos_type pos_else(20);
    grammar::ifnot_acceptor ifnot_acc1(pos, std::move(util::mkptr(new grammar::bool_literal(pos, true))));
    ifnot_acc1.accept_else(pos_else);
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_else_not_matches().size());
    ASSERT_EQ(pos_else, get_else_not_matches()[0].pos);

    clear_err();
    misc::pos_type pos_func(220);
    grammar::if_acceptor ifnot_acc2(pos, std::move(util::mkptr(new grammar::reference(pos, "tank"))));
    ifnot_acc2.accept_func(std::move(util::mkptr(new grammar::func_def(pos_func
                                                                      , "pylon"
                                                                      , std::vector<std::string>({ "zealot" })
                                                                      , std::move(grammar::block())))));
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_forbidden_funcs().size());
    ASSERT_EQ(pos_func, get_forbidden_funcs()[0].pos);
    ASSERT_EQ("pylon", get_forbidden_funcs()[0].name);
}

TEST_F(AcceptorTest, FuncAcceptor)
{
    misc::pos_type pos(5);
    test_acceptor receiver;

    grammar::func_def_acceptor func_acc0(pos, "func1", std::vector<std::string>({ "Duke", "Duran" }));
    func_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::float_literal(pos, "21.37")))))));
    func_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::var_def(pos, "SonOfKorhal", std::move(
                            util::mkptr(new grammar::int_literal(pos, "20110116")))))));

    func_acc0.deliver_to(util::mkref(receiver));
    ASSERT_FALSE(bool(receiver.stmt));
    ASSERT_TRUE(bool(receiver.func));
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));
    receiver.compile(*scope);

    data_tree::expect_one()
    (SCOPE)
        (pos, FUNC_DECL, "func1")
            (pos, PARAMETER, "Duke")
            (pos, PARAMETER, "Duran")
        (SCOPE)
                (pos, FLOATING, "21.37")
            (pos, ARITHMETICS)
                (pos, INTEGER, "20110116")
            (pos, VAR_DEF, "SonOfKorhal")
    ;
    ASSERT_FALSE(error::has_error());

    misc::pos_type pos_else(10);
    grammar::func_def_acceptor func_acc1(pos, "func2", std::vector<std::string>({ "Mengsk" }));
    func_acc1.accept_else(pos_else);
    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_else_not_matches().size());
    ASSERT_EQ(pos_else, get_else_not_matches()[0].pos);
}

TEST_F(AcceptorTest, FuncAccNested)
{
    misc::pos_type pos(5);
    test_acceptor receiver;

    grammar::func_def_acceptor func_acc0(pos, "funca", std::vector<std::string>({ "firebat", "ghost" }));
    func_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::float_literal(pos, "22.15")))))));
    func_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::var_def(pos, "medic", std::move(
                            util::mkptr(new grammar::reference(pos, "wraith")))))));

    grammar::func_def_acceptor func_acc1(pos, "funca", std::vector<std::string>({ "vulture" }));
    func_acc1.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::reference(pos, "goliath")))))));

    func_acc1.deliver_to(util::mkref(func_acc0));
    func_acc0.deliver_to(util::mkref(receiver));
    ASSERT_FALSE(bool(receiver.stmt));
    ASSERT_TRUE(bool(receiver.func));
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));
    receiver.compile(*scope);

    data_tree::expect_one()
    (SCOPE)
        (pos, FUNC_DECL, "funca")
            (pos, PARAMETER, "firebat")
            (pos, PARAMETER, "ghost")
        (SCOPE)
            (pos, FUNC_DECL, "funca")
                (pos, PARAMETER, "vulture")
            (SCOPE)
                    (pos, REFERENCE, "goliath")
                (pos, ARITHMETICS)
                (pos, FLOATING, "22.15")

            (pos, ARITHMETICS)
                (pos, REFERENCE, "wraith")
            (pos, VAR_DEF, "medic")
    ;
    ASSERT_FALSE(error::has_error());
}
