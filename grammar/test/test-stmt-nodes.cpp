#include <list>
#include <gtest/gtest.h>

#include "test-common.h"
#include "../stmt-nodes.h"
#include "../expr-nodes.h"
#include "../../test/phony-errors.h"

using namespace test;

typedef grammar_test StmtNodesTest;

TEST_F(StmtNodesTest, Arithmetics)
{
    misc::pos_type pos(1);
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));
    grammar::arithmetics arith0(pos, std::move(util::mkptr(new grammar::int_literal(pos, "1840"))));
    grammar::arithmetics arith1(pos, std::move(util::mkptr(new grammar::bool_literal(pos, false))));
    arith0.compile(*scope);
    arith1.compile(*scope);

    data_tree::expect_one()
    (SCOPE)
            (pos, INTEGER, "1840")
        (pos, ARITHMETICS)
            (pos, BOOLEAN, "false")
        (pos, ARITHMETICS)
    ;
}

TEST_F(StmtNodesTest, VarDef)
{
    misc::pos_type pos(2);
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));
    grammar::var_def def0(pos, "Shinji", std::move(util::mkptr(new grammar::float_literal(pos, "18.47"))));
    grammar::var_def def1(pos, "Asuka", std::move(util::mkptr(new grammar::reference(pos, "tsundere"))));
    def0.compile(*scope);
    def1.compile(*scope);

    data_tree::expect_one()
    (SCOPE)
            (pos, FLOATING, "18.47")
        (pos, VAR_DEF, "Shinji")
            (pos, REFERENCE, "tsundere")
        (pos, VAR_DEF, "Asuka")
    ;
}

TEST_F(StmtNodesTest, Returns)
{
    misc::pos_type pos(3);
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));
    grammar::func_ret ret0(pos, std::move(util::mkptr(new grammar::reference(pos, "KaworuNagisa"))));
    grammar::func_ret_nothing ret1(pos);
    ret0.compile(*scope);
    ret1.compile(*scope);

    data_tree::expect_one()
    (SCOPE)
            (pos, REFERENCE, "KaworuNagisa")
        (pos, RETURN)
        (pos, RETURN_NOTHING)
    ;
}

TEST_F(StmtNodesTest, Block)
{
    misc::pos_type pos(4);
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));
    grammar::block block;
    block.add_stmt(std::move(
                util::mkptr(new grammar::var_def(pos, "Misato", std::move(
                            util::mkptr(new grammar::reference(pos, "Katsuragi")))))));
    block.add_stmt(std::move(util::mkptr(new grammar::func_ret_nothing(pos))));
    block.compile(*scope);

    data_tree::expect_one()
    (SCOPE)
            (pos, REFERENCE, "Katsuragi")
        (pos, VAR_DEF, "Misato")
        (pos, RETURN_NOTHING)
    ;
}

TEST_F(StmtNodesTest, BlockMove)
{
    misc::pos_type pos(5);
    util::sptr<proto::scope> scope_a(std::move(proto::scope::global_scope()));
    grammar::block block0;
    block0.add_stmt(std::move(
                util::mkptr(new grammar::var_def(pos, "Akagi", std::move(
                            util::mkptr(new grammar::reference(pos, "Ibuki")))))));
    block0.add_stmt(std::move(util::mkptr(new grammar::func_ret_nothing(pos))));
    block0.compile(*scope_a);

    data_tree::expect_one()
    (SCOPE)
            (pos, REFERENCE, "Ibuki")
        (pos, VAR_DEF, "Akagi")
        (pos, RETURN_NOTHING)
    ;
    data_tree::verify();

    grammar::block block1(std::move(block0));
    util::sptr<proto::scope> scope_b(std::move(proto::scope::global_scope()));
    block0.compile(*scope_b);
    data_tree::expect_one()(SCOPE);
    data_tree::verify();

    util::sptr<proto::scope> scope_c(std::move(proto::scope::global_scope()));
    block1.compile(*scope_c);
    data_tree::expect_one()
    (SCOPE)
            (pos, REFERENCE, "Ibuki")
        (pos, VAR_DEF, "Akagi")
        (pos, RETURN_NOTHING)
    ;
}

TEST_F(StmtNodesTest, Branch)
{
    misc::pos_type pos(6);
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));
    grammar::branch(pos
                  , std::move(util::mkptr(new grammar::bool_literal(pos, true)))
                  , std::move(grammar::block())
                  , std::move(grammar::block()))
        .compile(*scope);

    grammar::block block0;
    block0.add_stmt(std::move(util::mkptr(
                new grammar::arithmetics(pos, std::move(util::mkptr(new grammar::reference(pos, "Kaji")))))));
    block0.add_stmt(std::move(util::mkptr(new grammar::func_ret_nothing(pos))));
    grammar::branch(pos
                  , std::move(util::mkptr(new grammar::bool_literal(pos, false)))
                  , std::move(block0)
                  , std::move(grammar::block()))
        .compile(*scope);

    grammar::block block1;
    block1.add_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::reference(pos, "Ryoji")))))));
    block1.add_stmt(std::move(util::mkptr(new grammar::func_ret_nothing(pos))));
    grammar::branch(pos
                  , std::move(util::mkptr(new grammar::bool_literal(pos, true)))
                  , std::move(grammar::block())
                  , std::move(block1))
        .compile(*scope);

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
        .compile(*scope);

    data_tree::expect_one()
    (SCOPE)
        (SCOPE)
        (SCOPE)
        (pos, BOOLEAN, "true")
        (pos, BRANCH)

        (SCOPE)
                (pos, REFERENCE, "Kaji")
            (pos, ARITHMETICS)
            (pos, RETURN_NOTHING)
        (SCOPE)
        (pos, BOOLEAN, "false")
        (pos, BRANCH)

        (SCOPE)
        (SCOPE)
                (pos, REFERENCE, "Ryoji")
            (pos, ARITHMETICS)
            (pos, RETURN_NOTHING)
        (pos, BOOLEAN, "true")
        (pos, BRANCH)

        (SCOPE)
                (pos, INTEGER, "7")
            (pos, ARITHMETICS)
            (pos, RETURN_NOTHING)
        (SCOPE)
                (pos, REFERENCE, "betsuni")
            (pos, RETURN)
        (pos, BOOLEAN, "false")
        (pos, BRANCH)
    ;
}

TEST_F(StmtNodesTest, FuncDefs)
{
    misc::pos_type pos(8);
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));
    grammar::func_def func0(pos, "func0", std::vector<std::string>(), std::move(grammar::block()));
    util::sref<proto::function> templ0 = func0.declare(*scope);
    func0.compile(templ0);

    grammar::block body;
    body.add_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::reference(pos, "Kuroi")))))));
    body.add_stmt(std::move(util::mkptr(new grammar::func_ret_nothing(pos))));
    grammar::func_def func1(pos
                          , "func1"
                          , std::vector<std::string>({ "Konata", "Kagami", "Tsukasa", "Miyuki" })
                          , std::move(body));
    util::sref<proto::function> templ1 = func1.declare(*scope);
    func1.compile(templ1);

    data_tree::expect_one()
    (SCOPE)
        (pos, FUNC_DECL, "func0")
        (SCOPE)

        (pos, FUNC_DECL, "func1")
            (pos, PARAMETER, "Konata")
            (pos, PARAMETER, "Kagami")
            (pos, PARAMETER, "Tsukasa")
            (pos, PARAMETER, "Miyuki")
        (SCOPE)
                (pos, REFERENCE, "Kuroi")
            (pos, ARITHMETICS)
            (pos, RETURN_NOTHING)
    ;
}

TEST_F(StmtNodesTest, Mixed)
{
    misc::pos_type pos(9);
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));

    grammar::block block_nested;
    block_nested.add_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::int_literal(pos, "9")))))));
    util::sptr<grammar::func_def> func_nested0(
            new grammar::func_def(pos, "funcn", std::vector<std::string>({ "SOS" }), std::move(block_nested)));
    util::sptr<grammar::func_def> func_nested1(
            new grammar::func_def(pos, "funcn", std::vector<std::string>(), std::move(grammar::block())));

    grammar::block body;
    body.add_stmt(std::move(
                util::mkptr(new grammar::arithmetics(pos, std::move(
                            util::mkptr(new grammar::reference(pos, "Kyon")))))));
    body.add_func(std::move(func_nested0));
    body.add_func(std::move(func_nested1));
    body.add_stmt(std::move(util::mkptr(new grammar::func_ret_nothing(pos))));

    grammar::func_def func(pos
                         , "funco"
                         , std::vector<std::string>({ "Suzumiya", "Koizumi", "Nagato", "Asahina" })
                         , std::move(body));
    util::sref<proto::function> templ = func.declare(*scope);
    func.compile(templ);

    data_tree::expect_one()
    (SCOPE)
        (pos, FUNC_DECL, "funco")
            (pos, PARAMETER, "Suzumiya")
            (pos, PARAMETER, "Koizumi")
            (pos, PARAMETER, "Nagato")
            (pos, PARAMETER, "Asahina")
        (SCOPE)
            (pos, FUNC_DECL, "funcn")
                (pos, PARAMETER, "SOS")
            (SCOPE)
            (pos, FUNC_DECL, "funcn")
            (SCOPE)
                    (pos, INTEGER, "9")
                (pos, ARITHMETICS)

                (pos, REFERENCE, "Kyon")
            (pos, ARITHMETICS)
            (pos, RETURN_NOTHING)
    ;
}
