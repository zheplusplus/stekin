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
#include "../../test/common.h"
#include "../../test/phony-errors.h"

using namespace test;

struct TestAcceptor
    : public grammar::Acceptor
{
    TestAcceptor()
        : grammar::Acceptor(misc::position(0))
        , filter(new flchk::filter)
    {}

    void accept_stmt(util::sptr<grammar::Statement const> s)
    {
        stmt = std::move(s);
    }

    void accept_func(util::sptr<grammar::Function const> f)
    {
        func = std::move(f);
    }

    void compile()
    {
        grammar::Block Block;
        if (bool(stmt)) {
            Block.add_stmt(std::move(stmt));
        }
        if (bool(func)) {
            Block.add_func(std::move(func));
        }
        filter = std::move(Block.compile(std::move(filter)));
    }

    util::sptr<grammar::Statement const> stmt;
    util::sptr<grammar::Function const> func;
    util::sptr<flchk::filter> filter;

    void deliver_to(util::sref<grammar::Acceptor>) {}
};

typedef grammar_test AcceptorTest;

TEST_F(AcceptorTest, IfAcceptor)
{
    misc::position pos(1);
    misc::position pos_head(100);
    misc::position pos_else(101);
    TestAcceptor receiver;

    grammar::IfAcceptor Acceptor_a(pos_head, std::move(util::mkptr(new grammar::IntLiteral(pos_head, "0"))));
    Acceptor_a.accept_stmt(std::move(util::mkptr(new grammar::func_ret_nothing(pos))));

    Acceptor_a.accept_else(pos_else);
    ASSERT_FALSE(error::hasError());
    Acceptor_a.accept_stmt(std::move(
                util::mkptr(new grammar::var_def(pos, "Hyperion", std::move(
                            util::mkptr(new grammar::Reference(pos, "Raynor")))))));

    Acceptor_a.deliver_to(util::mkref(receiver));
    ASSERT_TRUE(bool(receiver.stmt));
    ASSERT_FALSE(bool(receiver.func));
    receiver.compile();

    grammar::IfAcceptor Acceptor_b(pos_head, std::move(util::mkptr(new grammar::IntLiteral(pos_head, "1"))));
    Acceptor_b.accept_stmt(std::move(
                util::mkptr(new grammar::func_ret(pos, std::move(
                            util::mkptr(new grammar::Reference(pos, "Karrigan")))))));

    Acceptor_b.deliver_to(util::mkref(receiver));
    ASSERT_TRUE(bool(receiver.stmt));
    ASSERT_FALSE(bool(receiver.func));
    receiver.compile();

    grammar::IfAcceptor Acceptor_c(pos_head, std::move(util::mkptr(new grammar::IntLiteral(pos_head, "2"))));
    Acceptor_c.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::BoolLiteral(pos, false)))))));
    Acceptor_c.accept_else(pos_else);
    ASSERT_FALSE(error::hasError());

    Acceptor_c.deliver_to(util::mkref(receiver));
    ASSERT_TRUE(bool(receiver.stmt));
    ASSERT_FALSE(bool(receiver.func));
    receiver.compile();

    grammar::IfAcceptor Acceptor_d(pos_head, std::move(util::mkptr(new grammar::IntLiteral(pos_head, "3"))));
    Acceptor_d.accept_else(pos_else);
    ASSERT_FALSE(error::hasError());
    Acceptor_d.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::FloatLiteral(pos, "20.54")))))));

    Acceptor_d.deliver_to(util::mkref(receiver));
    ASSERT_TRUE(bool(receiver.stmt));
    ASSERT_FALSE(bool(receiver.func));
    receiver.compile();
    receiver.filter->deliver().compile(nulscope);

    DataTree::expectOne()
        (BLOCK_BEGIN)
        (pos_head, BRANCH)
        (pos_head, INTEGER, "0")
        (CONSEQUENCE)
            (BLOCK_BEGIN)
            (pos, RETURN_NOTHING)
            (BLOCK_END)
        (ALTERNATIVE)
            (BLOCK_BEGIN)
            (pos, VAR_DEF, "Hyperion" + VAR_DEF_FILTERED)
                (pos, REFERENCE, "Raynor")
            (BLOCK_END)

        (pos_head, BRANCH_CONSQ_ONLY)
        (pos_head, INTEGER, "1")
        (CONSEQUENCE)
            (BLOCK_BEGIN)
            (pos, RETURN)
                (pos, REFERENCE, "Karrigan")
            (BLOCK_END)

        (pos_head, BRANCH)
        (pos_head, INTEGER, "2")
        (CONSEQUENCE)
            (BLOCK_BEGIN)
            (pos, ARITHMETICS)
                (pos, BOOLEAN, "false")
            (BLOCK_END)
        (ALTERNATIVE)
            (BLOCK_BEGIN)
            (BLOCK_END)

        (pos_head, BRANCH)
        (pos_head, INTEGER, "3")
        (CONSEQUENCE)
            (BLOCK_BEGIN)
            (BLOCK_END)
        (ALTERNATIVE)
            (BLOCK_BEGIN)
            (pos, ARITHMETICS)
                (pos, FLOATING, "20.54")
            (BLOCK_END)
        (BLOCK_END)
    ;
    ASSERT_FALSE(error::hasError());
}

TEST_F(AcceptorTest, IfAcceptorError)
{
    misc::position pos(2);
    misc::position pos_head(200);
    misc::position pos_else(201);
    grammar::IfAcceptor Acceptor_a(pos_head, std::move(util::mkptr(new grammar::IntLiteral(pos_head, "0"))));
    Acceptor_a.accept_else(pos);
    ASSERT_FALSE(error::hasError());
    Acceptor_a.accept_else(pos_else);
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, get_if_matcheds().size());
    ASSERT_EQ(pos, get_if_matcheds()[0].prev_pos);
    ASSERT_EQ(pos_else, get_if_matcheds()[0].this_pos);
}

TEST_F(AcceptorTest, IfNotAcceptor)
{
    misc::position pos(3);
    TestAcceptor receiver;

    grammar::IfnotAcceptor ifnot_acc0(pos, std::move(util::mkptr(new grammar::BoolLiteral(pos, false))));
    ifnot_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::var_def(pos, "SCV", std::move(
                            util::mkptr(new grammar::IntLiteral(pos, "60")))))));
    ifnot_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::Reference(pos, "Marine")))))));

    ifnot_acc0.deliver_to(util::mkref(receiver));
    ASSERT_TRUE(bool(receiver.stmt));
    ASSERT_FALSE(bool(receiver.func));
    receiver.compile();
    receiver.filter->deliver().compile(nulscope);

    DataTree::expectOne()
        (BLOCK_BEGIN)
        (pos, BRANCH_ALTER_ONLY)
        (pos, BOOLEAN, "false")
        (ALTERNATIVE)
            (BLOCK_BEGIN)
            (pos, VAR_DEF, "SCV" + VAR_DEF_FILTERED)
                (pos, INTEGER, "60")
            (pos, ARITHMETICS)
                (pos, REFERENCE, "Marine")
            (BLOCK_END)
        (BLOCK_END)
    ;
    ASSERT_FALSE(error::hasError());

    misc::position pos_else(20);
    grammar::IfnotAcceptor ifnot_acc1(pos, std::move(util::mkptr(new grammar::BoolLiteral(pos, true))));
    ifnot_acc1.accept_else(pos_else);
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, get_else_not_matches().size());
    ASSERT_EQ(pos_else, get_else_not_matches()[0].pos);
}

TEST_F(AcceptorTest, FuncAcceptor)
{
    misc::position pos(5);
    TestAcceptor receiver;

    grammar::FunctionAcceptor func_acc0(pos, "func1", std::vector<std::string>({ "Duke", "Duran" }));
    func_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::FloatLiteral(pos, "21.37")))))));
    func_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::var_def(pos, "SonOfKorhal", std::move(
                            util::mkptr(new grammar::IntLiteral(pos, "20110116")))))));

    func_acc0.deliver_to(util::mkref(receiver));
    ASSERT_FALSE(bool(receiver.stmt));
    ASSERT_TRUE(bool(receiver.func));
    receiver.compile();
    receiver.filter->deliver().compile(nulscope);

    DataTree::expectOne()
        (BLOCK_BEGIN)
        (pos, FUNC_DEF, "func1")
            (pos, PARAMETER, "Duke")
            (pos, PARAMETER, "Duran")
            (BLOCK_BEGIN)
            (pos, ARITHMETICS)
                (pos, FLOATING, "21.37")
            (pos, VAR_DEF, "SonOfKorhal")
                (pos, INTEGER, "20110116")
            (BLOCK_END)
        (BLOCK_END)
    ;
    ASSERT_FALSE(error::hasError());

    misc::position pos_else(10);
    grammar::FunctionAcceptor func_acc1(pos, "func2", std::vector<std::string>({ "Mengsk" }));
    func_acc1.accept_else(pos_else);
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, get_else_not_matches().size());
    ASSERT_EQ(pos_else, get_else_not_matches()[0].pos);
}

TEST_F(AcceptorTest, FuncAccNested)
{
    misc::position pos(5);
    TestAcceptor receiver;

    grammar::FunctionAcceptor func_acc0(pos, "funca", std::vector<std::string>({ "firebat", "ghost" }));
    func_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::FloatLiteral(pos, "22.15")))))));
    func_acc0.accept_stmt(std::move(
                util::mkptr(new grammar::var_def(pos, "medic", std::move(
                            util::mkptr(new grammar::Reference(pos, "wraith")))))));

    grammar::FunctionAcceptor func_acc1(pos, "funca", std::vector<std::string>({ "vulture" }));
    func_acc1.accept_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::Reference(pos, "goliath")))))));

    func_acc1.deliver_to(util::mkref(func_acc0));
    func_acc0.deliver_to(util::mkref(receiver));
    ASSERT_FALSE(bool(receiver.stmt));
    ASSERT_TRUE(bool(receiver.func));
    receiver.compile();
    receiver.filter->deliver().compile(nulscope);

    DataTree::expectOne()
        (BLOCK_BEGIN)
        (pos, FUNC_DEF, "funca")
            (pos, PARAMETER, "firebat")
            (pos, PARAMETER, "ghost")
            (BLOCK_BEGIN)
            (pos, FUNC_DEF, "funca")
                (pos, PARAMETER, "vulture")
                (BLOCK_BEGIN)
                (pos, ARITHMETICS)
                    (pos, REFERENCE, "goliath")
                (BLOCK_END)
            (pos, ARITHMETICS)
                (pos, FLOATING, "22.15")
            (pos, VAR_DEF, "medic")
                (pos, REFERENCE, "wraith")
            (BLOCK_END)
        (BLOCK_END)
    ;
    ASSERT_FALSE(error::hasError());
}
