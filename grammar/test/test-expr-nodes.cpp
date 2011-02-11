#include <list>
#include <gtest/gtest.h>

#include "test-common.h"
#include "../expr-nodes.h"
#include "../../test/phony-errors.h"

using namespace test;

typedef grammar_test ExprNodesTest;

TEST_F(ExprNodesTest, Literals)
{
    misc::pos_type pos(1);
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));
    grammar::int_literal int0(pos, "20110116");
    int0.compile(*scope);
    grammar::float_literal float0(pos, "19.50");
    float0.compile(*scope);
    grammar::bool_literal bool0(pos, true);
    bool0.compile(*scope);
    grammar::int_literal int1(pos, "441499");
    int1.compile(*scope);
    grammar::float_literal float1(pos, "0.1950");
    float1.compile(*scope);
    grammar::bool_literal bool1(pos, false);
    bool1.compile(*scope);

    data_tree::expect_one()
    (SCOPE)
        (pos, INTEGER, "20110116")
        (pos, FLOATING, "19.50")
        (pos, BOOLEAN, "true")
        (pos, INTEGER, "441499")
        (pos, FLOATING, "0.1950")
        (pos, BOOLEAN, "false")
    ;
}

TEST_F(ExprNodesTest, Reference)
{
    misc::pos_type pos(1);
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));
    grammar::reference ref0(pos, "a20110116");
    ref0.compile(*scope);
    grammar::reference ref1(pos, "b1950");
    ref1.compile(*scope);

    data_tree::expect_one()
    (SCOPE)
        (pos, REFERENCE, "a20110116")
        (pos, REFERENCE, "b1950")
    ;
}

TEST_F(ExprNodesTest, Operations)
{
    misc::pos_type pos(2);
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));
    grammar::binary_op binary0(pos
                             , new grammar::int_literal(pos, "1")
                             , "+"
                             , new grammar::float_literal(pos, "11235.8"));
    grammar::binary_op binary1(pos
                             , new grammar::float_literal(pos, "1.12358")
                             , "<="
                             , new grammar::int_literal(pos, "2357111317"));

    grammar::pre_unary_op pre_unary0(pos, "+", new grammar::float_literal(pos, ".13"));
    grammar::pre_unary_op pre_unary1(pos
                                   , "-"
                                   , new grammar::binary_op(pos
                                                          , new grammar::reference(pos, "wasureru")
                                                          , "%"
                                                          , new grammar::int_literal(pos, "1")));

    grammar::conjunction conj(pos, new grammar::bool_literal(pos, true), new grammar::reference(pos, "koto"));
    grammar::disjunction disj(pos, new grammar::bool_literal(pos, false), new grammar::int_literal(pos, "2"));
    grammar::negation nega(pos, new grammar::float_literal(pos, "1954.0112"));

    binary0.compile(*scope);
    binary1.compile(*scope);
    pre_unary0.compile(*scope);
    pre_unary1.compile(*scope);
    conj.compile(*scope);
    disj.compile(*scope);
    nega.compile(*scope);

    data_tree::expect_one()
    (SCOPE)
            (pos, FLOATING, "11235.8")
            (pos, INTEGER, "1")
        (pos, BINARY_OP, "+")
            (pos, INTEGER, "2357111317")
            (pos, FLOATING, "1.12358")
        (pos, BINARY_OP, "<=")
            (pos, FLOATING, ".13")
        (pos, PRE_UNARY_OP, "+")
                (pos, INTEGER, "1")
                (pos, REFERENCE, "wasureru")
            (pos, BINARY_OP, "%")
        (pos, PRE_UNARY_OP, "-")
            (pos, REFERENCE, "koto")
            (pos, BOOLEAN, "true")
        (pos, BINARY_OP, "&&")
            (pos, INTEGER, "2")
            (pos, BOOLEAN, "false")
        (pos, BINARY_OP, "||")
            (pos, FLOATING, "1954.0112")
        (pos, PRE_UNARY_OP, "!")
    ;
}

TEST_F(ExprNodesTest, Calls)
{
    misc::pos_type pos(3);
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));

    std::list<util::sptr<grammar::expr_base const>> params;
    grammar::call call0(pos, "fib", std::move(params));

    params.push_back(std::move(util::mkptr(new grammar::bool_literal(pos, false))));
    params.push_back(std::move(util::mkptr(
                    new grammar::pre_unary_op(pos, "-", new grammar::float_literal(pos, "11.11")))));
    params.push_back(std::move(util::mkptr(new grammar::negation(pos, new grammar::int_literal(pos, "21")))));
    params.push_back(std::move(util::mkptr(new grammar::reference(pos, "dareka_tasukete_kudasai"))));
    grammar::call call1(pos, "leap", std::move(params));

    call0.compile(*scope);
    call1.compile(*scope);

    data_tree::expect_one()
    (SCOPE)
        (pos, CALL, "fib", 0)
            (pos, BOOLEAN, "false")
                (pos, FLOATING, "11.11")
            (pos, PRE_UNARY_OP, "-")
                (pos, INTEGER, "21")
            (pos, PRE_UNARY_OP, "!")
            (pos, REFERENCE, "dareka_tasukete_kudasai")
        (pos, CALL, "leap", 4)
    ;
}
