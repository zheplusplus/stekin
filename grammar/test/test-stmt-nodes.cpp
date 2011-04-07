#include <list>
#include <gtest/gtest.h>

#include "test-common.h"
#include "../stmt-nodes.h"
#include "../expr-nodes.h"
#include "../function.h"
#include "../../flowcheck/node-base.h"
#include "../../flowcheck/filter.h"
#include "../../flowcheck/function.h"
#include "../../proto/node-base.h"
#include "../../test/phony-errors.h"

using namespace test;

typedef grammar_test StmtNodesTest;

TEST_F(StmtNodesTest, Arithmetics)
{
    misc::pos_type pos(1);
    util::sptr<flchk::filter> filter(std::move(util::mkmptr(new flchk::filter)));
    grammar::arithmetics arith0(pos, std::move(util::mkptr(new grammar::int_literal(pos, "1840"))));
    grammar::arithmetics arith1(pos, std::move(util::mkptr(new grammar::bool_literal(pos, false))));
    arith0.compile(*filter);
    arith1.compile(*filter);
    filter->deliver().compile(nulscope);

    data_tree::expect_one()
        (BLOCK_BEGIN)
        (pos, ARITHMETICS)
            (pos, INTEGER, "1840")
        (pos, ARITHMETICS)
            (pos, BOOLEAN, "false")
        (BLOCK_END)
    ;
}

TEST_F(StmtNodesTest, VarDef)
{
    misc::pos_type pos(2);
    util::sptr<flchk::filter> filter(std::move(util::mkmptr(new flchk::filter)));
    grammar::var_def def0(pos, "Shinji", std::move(util::mkptr(new grammar::float_literal(pos, "18.47"))));
    grammar::var_def def1(pos, "Asuka", std::move(util::mkptr(new grammar::reference(pos, "tsundere"))));
    def0.compile(*filter);
    def1.compile(*filter);
    filter->deliver().compile(nulscope);

    data_tree::expect_one()
        (BLOCK_BEGIN)
        (pos, VAR_DEF, "Shinji")
            (pos, FLOATING, "18.47")
        (pos, VAR_DEF, "Asuka")
            (pos, REFERENCE, "tsundere")
        (BLOCK_END)
    ;
}

TEST_F(StmtNodesTest, Returns)
{
    misc::pos_type pos(3);
    util::sptr<flchk::filter> filter(std::move(util::mkmptr(new flchk::filter)));
    grammar::func_ret ret0(pos, std::move(util::mkptr(new grammar::reference(pos, "KaworuNagisa"))));
    grammar::func_ret_nothing ret1(pos);
    ret0.compile(*filter);
    ret1.compile(*filter);
    filter->deliver().compile(nulscope);

    data_tree::expect_one()
        (BLOCK_BEGIN)
        (pos, RETURN)
            (pos, REFERENCE, "KaworuNagisa")
        (pos, RETURN_NOTHING)
        (BLOCK_END)
    ;
}

TEST_F(StmtNodesTest, Block)
{
    misc::pos_type pos(4);
    util::sptr<flchk::filter> filter(std::move(util::mkmptr(new flchk::filter)));
    grammar::block block;
    block.add_stmt(std::move(
                util::mkptr(new grammar::var_def(pos, "Misato", std::move(
                            util::mkptr(new grammar::reference(pos, "Katsuragi")))))));
    block.add_stmt(std::move(util::mkptr(new grammar::func_ret_nothing(pos))));
    block.compile(std::move(filter))->deliver().compile(nulscope);

    data_tree::expect_one()
        (BLOCK_BEGIN)
        (pos, VAR_DEF, "Misato")
            (pos, REFERENCE, "Katsuragi")
        (pos, RETURN_NOTHING)
        (BLOCK_END)
    ;
}

TEST_F(StmtNodesTest, Branch)
{
    misc::pos_type pos(6);
    util::sptr<flchk::filter> filter(std::move(util::mkmptr(new flchk::filter)));
    grammar::branch(pos
                  , std::move(util::mkptr(new grammar::bool_literal(pos, true)))
                  , std::move(grammar::block())
                  , std::move(grammar::block()))
        .compile(*filter);

    grammar::block block0;
    block0.add_stmt(std::move(util::mkptr(
                new grammar::arithmetics(pos, std::move(util::mkptr(new grammar::reference(pos, "Kaji")))))));
    block0.add_stmt(std::move(util::mkptr(new grammar::func_ret_nothing(pos))));
    grammar::branch_cons_only(pos
                            , std::move(util::mkptr(new grammar::bool_literal(pos, false)))
                            , std::move(block0))
        .compile(*filter);

    grammar::block block1;
    block1.add_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::reference(pos, "Ryoji")))))));
    block1.add_stmt(std::move(util::mkptr(new grammar::func_ret_nothing(pos))));
    grammar::branch_alt_only(pos
                           , std::move(util::mkptr(new grammar::bool_literal(pos, true)))
                           , std::move(block1))
        .compile(*filter);

    grammar::block block2;
    block2.add_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::int_literal(pos, "7")))))));
    block2.add_stmt(std::move(util::mkptr(new grammar::func_ret_nothing(pos))));
    grammar::block block3;
    block3.add_stmt(std::move(
                util::mkptr(new grammar::func_ret(pos, std::move(
                            util::mkptr(new grammar::reference(pos, "betsuni")))))));
    grammar::branch(pos
                  , std::move(util::mkptr(new grammar::bool_literal(pos, false)))
                  , std::move(block2)
                  , std::move(block3))
        .compile(*filter);
    filter->deliver().compile(nulscope);

    data_tree::expect_one()
        (BLOCK_BEGIN)
        (pos, BRANCH)
        (pos, BOOLEAN, "true")
        (CONSEQUENCE)
            (BLOCK_BEGIN)
            (BLOCK_END)
        (ALTERNATIVE)
            (BLOCK_BEGIN)
            (BLOCK_END)

        (pos, BRANCH_CONSQ_ONLY)
        (pos, BOOLEAN, "false")
        (CONSEQUENCE)
            (BLOCK_BEGIN)
            (pos, ARITHMETICS)
                (pos, REFERENCE, "Kaji")
            (pos, RETURN_NOTHING)
            (BLOCK_END)

        (pos, BRANCH_ALTER_ONLY)
        (pos, BOOLEAN, "true")
        (ALTERNATIVE)
            (BLOCK_BEGIN)
            (pos, ARITHMETICS)
                (pos, REFERENCE, "Ryoji")
            (pos, RETURN_NOTHING)
            (BLOCK_END)

        (pos, BRANCH)
        (pos, BOOLEAN, "false")
        (CONSEQUENCE)
            (BLOCK_BEGIN)
            (pos, ARITHMETICS)
                (pos, INTEGER, "7")
            (pos, RETURN_NOTHING)
            (BLOCK_END)
        (ALTERNATIVE)
            (BLOCK_BEGIN)
            (pos, RETURN)
                (pos, REFERENCE, "betsuni")
            (BLOCK_END)
        (BLOCK_END)
    ;
}

TEST_F(StmtNodesTest, Functions)
{
    misc::pos_type pos(8);
    util::sptr<flchk::filter> filter(std::move(util::mkmptr(new flchk::filter)));
    grammar::function func0(pos, "func0", std::vector<std::string>(), std::move(grammar::block()));
    func0.compile(*filter);

    grammar::block body;
    body.add_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::reference(pos, "Kuroi")))))));
    body.add_stmt(std::move(util::mkptr(new grammar::func_ret_nothing(pos))));
    grammar::function func1(pos
                          , "func1"
                          , std::vector<std::string>({ "Konata", "Kagami", "Tsukasa", "Miyuki" })
                          , std::move(body));
    func1.compile(*filter);
    filter->deliver().compile(nulscope);

    data_tree::expect_one()
        (BLOCK_BEGIN)
        (pos, FUNC_DEF, "func0")
            (BLOCK_BEGIN)
            (BLOCK_END)
        (pos, FUNC_DEF, "func1")
            (pos, PARAMETER, "Konata")
            (pos, PARAMETER, "Kagami")
            (pos, PARAMETER, "Tsukasa")
            (pos, PARAMETER, "Miyuki")
            (BLOCK_BEGIN)
            (pos, ARITHMETICS)
                (pos, REFERENCE, "Kuroi")
            (pos, RETURN_NOTHING)
            (BLOCK_END)
        (BLOCK_END)
    ;
}

TEST_F(StmtNodesTest, Mixed)
{
    misc::pos_type pos(9);
    util::sptr<flchk::filter> filter(std::move(util::mkmptr(new flchk::filter)));

    grammar::block block_nested;
    block_nested.add_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::int_literal(pos, "9")))))));
    util::sptr<grammar::function> func_nested0(
            new grammar::function(pos, "funcn", std::vector<std::string>({ "SOS" }), std::move(block_nested)));
    util::sptr<grammar::function> func_nested1(
            new grammar::function(pos, "funcn", std::vector<std::string>(), std::move(grammar::block())));

    grammar::block body;
    body.add_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::reference(pos, "Kyon")))))));
    body.add_func(std::move(func_nested0));
    body.add_func(std::move(func_nested1));
    body.add_stmt(std::move(util::mkptr(new grammar::func_ret_nothing(pos))));

    grammar::function func(pos
                         , "funco"
                         , std::vector<std::string>({ "Suzumiya", "Koizumi", "Nagato", "Asahina" })
                         , std::move(body));
    func.compile(*filter);
    filter->deliver().compile(nulscope);

    data_tree::expect_one()
        (BLOCK_BEGIN)
        (pos, FUNC_DEF, "funco")
            (pos, PARAMETER, "Suzumiya")
            (pos, PARAMETER, "Koizumi")
            (pos, PARAMETER, "Nagato")
            (pos, PARAMETER, "Asahina")
            (BLOCK_BEGIN)
            (pos, FUNC_DEF, "funcn")
                (pos, PARAMETER, "SOS")
                (BLOCK_BEGIN)
                (pos, ARITHMETICS)
                    (pos, INTEGER, "9")
                (BLOCK_END)
            (pos, FUNC_DEF, "funcn")
                (BLOCK_BEGIN)
                (BLOCK_END)
            (pos, ARITHMETICS)
                (pos, REFERENCE, "Kyon")
            (pos, RETURN_NOTHING)
            (BLOCK_END)
        (BLOCK_END)
    ;
}
