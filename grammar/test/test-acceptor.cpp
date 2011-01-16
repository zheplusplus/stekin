#include <gtest/gtest.h>

#include "phony-err-report.h"
#include "../acceptor.h"

using namespace test;

util::sptr<grammar::stmt_base const> actual_stmt;
util::sptr<grammar::func_def const> actual_func;

struct test_acceptor
    : public grammar::acceptor
{
    void accept_stmt(util::sptr<grammar::stmt_base const> stmt)
    {
        actual_stmt = std::move(stmt);
    }

    void accept_func(util::sptr<grammar::func_def const> func)
    {
        actual_func = std::move(func);
    }

    void deliver_to(util::sptr<grammar::acceptor>&) {}
};

struct AcceptorTest
    : public testing::Test
{
    void SetUp()
    {
        clear_err();
    }
};

TEST_F(AcceptorTest, IfAcceptor)
{
    misc::pos_type pos_head(1);
    grammar::if_acceptor acceptor_a(pos_head, std::move(util::mkptr(new grammar::int_literal(pos_head, "0"))));
    misc::pos_type pos_else(4);
    acceptor_a.accept_else(pos_else);
    ASSERT_FALSE(grammar::has_error());
}
