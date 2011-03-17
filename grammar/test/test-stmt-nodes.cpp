#include <list>
#include <gtest/gtest.h>

#include "test-common.h"
#include "../stmt-nodes.h"
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
    util::sptr<flchk::filter> filter(new flchk::filter);
    grammar::arithmetics arith0(pos, std::move(mkexpr()));
    grammar::arithmetics arith1(pos, std::move(mkexpr()));
    arith0.compile(*filter);
    arith1.compile(*filter);

    data_tree::expect_one()
        (pos, ARITHMETICS)
        (pos, ARITHMETICS)
    ;
}

TEST_F(StmtNodesTest, VarDef)
{
    misc::pos_type pos(2);
    util::sptr<flchk::filter> filter(new flchk::filter);
    grammar::var_def def0(pos, "Shinji", std::move(mkexpr()));
    grammar::var_def def1(pos, "Asuka", std::move(mkexpr()));
    def0.compile(*filter);
    def1.compile(*filter);

    data_tree::expect_one()
        (pos, VAR_DEF, "Shinji")
        (pos, VAR_DEF, "Asuka")
    ;
}

TEST_F(StmtNodesTest, Returns)
{
    misc::pos_type pos(3);
    util::sptr<flchk::filter> filter(new flchk::filter);
    grammar::func_ret ret0(pos, std::move(mkexpr()));
    grammar::func_ret_nothing ret1(pos);
    ret0.compile(*filter);
    ret1.compile(*filter);

    data_tree::expect_one()
        (pos, RETURN)
        (pos, RETURN_NOTHING)
    ;
}

TEST_F(StmtNodesTest, Block)
{
    misc::pos_type pos(4);
    grammar::block block;
    block.add_stmt(std::move(util::mkmptr(new grammar::var_def(pos, "Misato", std::move(mkexpr())))));
    block.add_stmt(std::move(util::mkmptr(new grammar::func_ret_nothing(pos))));
    block.compile(std::move(util::mkmptr(new flchk::filter)));

    data_tree::expect_one()
        (pos, VAR_DEF, "Misato")
        (pos, RETURN_NOTHING)
    ;
}

TEST_F(StmtNodesTest, Branch)
{
    misc::pos_type pos(6);
    util::sptr<flchk::filter> filter(new flchk::filter);
    grammar::branch(pos, std::move(mkexpr()), std::move(grammar::block()), std::move(grammar::block()))
        .compile(*filter);

    grammar::block block0;
    block0.add_stmt(std::move(util::mkmptr(new grammar::var_def(pos, "Akagi", std::move(mkexpr())))));
    block0.add_stmt(std::move(util::mkmptr(new grammar::func_ret_nothing(pos))));
    grammar::branch_cons_only(pos, std::move(mkexpr()), std::move(block0)).compile(*filter);

    grammar::block block1;
    block1.add_stmt(std::move(util::mkmptr(new grammar::arithmetics(pos, std::move(mkexpr())))));
    block1.add_stmt(std::move(util::mkmptr(new grammar::func_ret_nothing(pos))));
    grammar::branch_alt_only(pos, std::move(mkexpr()), std::move(block1)).compile(*filter);

    grammar::block block2;
    block2.add_stmt(std::move(util::mkmptr(new grammar::arithmetics(pos, std::move(mkexpr())))));
    block2.add_stmt(std::move(util::mkmptr(new grammar::func_ret_nothing(pos))));
    grammar::block block3;
    block3.add_stmt(std::move(util::mkmptr(new grammar::func_ret(pos, std::move(mkexpr())))));
    grammar::branch(pos, std::move(mkexpr()), std::move(block2), std::move(block3)).compile(*filter);

    data_tree::expect_one()
        (pos, BRANCH)

            (pos, VAR_DEF_FILTERED, "Akagi")
            (pos, RETURN_NOTHING)
        (pos, BRANCH_CONSQ_ONLY)

            (pos, ARITHMETICS)
            (pos, RETURN_NOTHING)
        (pos, BRANCH_ALTER_ONLY)

            (pos, RETURN)

            (pos, ARITHMETICS)
            (pos, RETURN_NOTHING)
        (pos, BRANCH)
    ;
}

TEST_F(StmtNodesTest, FuncDefs)
{
    misc::pos_type pos(8);
    util::sptr<flchk::filter> filter(new flchk::filter);
    grammar::function func0(pos, "func0", std::vector<std::string>(), std::move(grammar::block()));
    func0.compile(*filter);

    grammar::block body;
    body.add_stmt(std::move(util::mkmptr(new grammar::arithmetics(pos, std::move(mkexpr())))));
    body.add_stmt(std::move(util::mkmptr(new grammar::func_ret_nothing(pos))));
    grammar::function func1(pos
                          , "func1"
                          , std::vector<std::string>({ "Konata", "Kagami", "Tsukasa", "Miyuki" })
                          , std::move(body));
    func1.compile(*filter);

    data_tree::expect_one()
        (pos, FUNC_DEF, "func0")

            (pos, ARITHMETICS)
            (pos, RETURN_NOTHING)
        (pos, FUNC_DEF, "func1")
            (pos, PARAMETER, "Konata")
            (pos, PARAMETER, "Kagami")
            (pos, PARAMETER, "Tsukasa")
            (pos, PARAMETER, "Miyuki")
    ;
}

TEST_F(StmtNodesTest, Mixed)
{
    misc::pos_type pos(9);
    util::sptr<flchk::filter> filter(new flchk::filter);

    grammar::block block_nested;
    block_nested.add_stmt(std::move(util::mkmptr(new grammar::arithmetics(pos, std::move(mkexpr())))));
    util::sptr<grammar::function> func_nested0(
            new grammar::function(pos, "funcn", std::vector<std::string>({ "SOS" }), std::move(block_nested)));
    util::sptr<grammar::function> func_nested1(
            new grammar::function(pos, "funcn", std::vector<std::string>(), std::move(grammar::block())));

    grammar::block body;
    body.add_stmt(std::move(util::mkmptr(new grammar::arithmetics(pos, std::move(mkexpr())))));
    body.add_func(std::move(func_nested0));
    body.add_stmt(std::move(util::mkmptr(new grammar::func_ret_nothing(pos))));
    body.add_func(std::move(func_nested1));

    grammar::function func(pos
                         , "funco"
                         , std::vector<std::string>({ "Suzumiya", "Koizumi", "Nagato", "Asahina" })
                         , std::move(body));
    func.compile(*filter);

    data_tree::expect_one()
                (pos, ARITHMETICS)
            (pos, FUNC_DEF, "funcn")
                (pos, PARAMETER, "SOS")
            (pos, FUNC_DEF, "funcn")

            (pos, ARITHMETICS)
            (pos, RETURN_NOTHING)
        (pos, FUNC_DEF, "funco")
            (pos, PARAMETER, "Suzumiya")
            (pos, PARAMETER, "Koizumi")
            (pos, PARAMETER, "Nagato")
            (pos, PARAMETER, "Asahina")
    ;
}
