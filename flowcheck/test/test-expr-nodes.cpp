#include <vector>
#include <gtest/gtest.h>

#include "test-common.h"
#include "../expr-nodes.h"
#include "../../proto/node-base.h"
#include "../../proto/function.h"
#include "../../proto/global-scope.h"
#include "../../instance/node-base.h"
#include "../../test/phony-errors.h"

using namespace test;

typedef flowcheck_test ExprNodesTest;

TEST_F(ExprNodesTest, Literals)
{
    misc::pos_type pos(1);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));
    flchk::int_literal int0(pos, "20110116");
    int0.compile(*scope)->inst(nul_inst_scope);
    flchk::float_literal float0(pos, "19.50");
    float0.compile(*scope)->inst(nul_inst_scope);
    flchk::bool_literal bool0(pos, true);
    bool0.compile(*scope)->inst(nul_inst_scope);
    flchk::int_literal int1(pos, "441499");
    int1.compile(*scope)->inst(nul_inst_scope);
    flchk::float_literal float1(pos, "0.1950");
    float1.compile(*scope)->inst(nul_inst_scope);
    flchk::bool_literal bool1(pos, false);
    bool1.compile(*scope)->inst(nul_inst_scope);

    data_tree::expect_one()
        (pos, INTEGER, "20110116")
        (pos, FLOATING, "19.5")
        (pos, BOOLEAN, "true")
        (pos, INTEGER, "441499")
        (pos, FLOATING, "0.195")
        (pos, BOOLEAN, "false")
    ;
}

TEST_F(ExprNodesTest, Reference)
{
    misc::pos_type pos(1);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));
    flchk::reference ref0(pos, "a20110116");
    ref0.compile(*scope)->inst(nul_inst_scope);
    flchk::reference ref1(pos, "b1950");
    ref1.compile(*scope)->inst(nul_inst_scope);

    data_tree::expect_one()
        (pos, VAR_REF, "a20110116")
        (pos, VAR_REF, "b1950")
    ;
}

TEST_F(ExprNodesTest, Operations)
{
    misc::pos_type pos(2);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));
    flchk::binary_op binary0(pos
                           , std::move(util::mkptr(new flchk::int_literal(pos, "1")))
                           , "+"
                           , std::move(util::mkptr(new flchk::float_literal(pos, "11235.8"))));
    flchk::binary_op binary1(pos
                           , std::move(util::mkptr(new flchk::float_literal(pos, "1.12358")))
                           , "<="
                           , std::move(util::mkptr(new flchk::int_literal(pos, "2357111317"))));

    flchk::pre_unary_op pre_unary0(pos, "+", std::move(util::mkptr(new flchk::float_literal(pos, ".13"))));
    flchk::pre_unary_op pre_unary1(
            pos
          , "-"
          , std::move(util::mkptr(
                    new flchk::binary_op(pos
                                       , std::move(util::mkptr(new flchk::reference(pos, "wasureru")))
                                       , "%"
                                       , std::move(util::mkptr(new flchk::int_literal(pos, "1")))))));

    flchk::conjunction conj(pos
                          , std::move(util::mkptr(new flchk::bool_literal(pos, true)))
                          , std::move(util::mkptr(new flchk::reference(pos, "koto"))));
    flchk::disjunction disj(pos
                          , std::move(util::mkptr(new flchk::bool_literal(pos, false)))
                          , std::move(util::mkptr(new flchk::int_literal(pos, "2"))));
    flchk::negation nega(pos, std::move(util::mkptr(new flchk::float_literal(pos, "1954.01"))));

    binary0.compile(*scope)->inst(nul_inst_scope);
    binary1.compile(*scope)->inst(nul_inst_scope);
    pre_unary0.compile(*scope)->inst(nul_inst_scope);
    pre_unary1.compile(*scope)->inst(nul_inst_scope);
    conj.compile(*scope)->inst(nul_inst_scope);
    disj.compile(*scope)->inst(nul_inst_scope);
    nega.compile(*scope)->inst(nul_inst_scope);

    data_tree::expect_one()
        (pos, BINARY_OP, "+")
            (pos, INTEGER, "1")
            (pos, FLOATING, "11235.8")
        (pos, BINARY_OP, "<=")
            (pos, FLOATING, "1.12358")
            (pos, INTEGER, "2357111317")
        (pos, PRE_UNARY_OP, "+")
            (pos, FLOATING, "0.13")
        (pos, PRE_UNARY_OP, "-")
            (pos, BINARY_OP, "%")
                (pos, VAR_REF, "wasureru")
                (pos, INTEGER, "1")
        (pos, BINARY_OP, "&&")
            (pos, BOOLEAN, "true")
            (pos, VAR_REF, "koto")
        (pos, BINARY_OP, "||")
            (pos, BOOLEAN, "false")
            (pos, INTEGER, "2")
        (pos, PRE_UNARY_OP, "!")
            (pos, FLOATING, "1954.01")
    ;
}

TEST_F(ExprNodesTest, Calls)
{
    misc::pos_type pos(3);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));

    std::vector<util::sptr<flchk::expr_base const>> params;
    flchk::call call0(pos, "fib", std::move(params));

    params.push_back(std::move(util::mkptr(new flchk::bool_literal(pos, false))));
    params.push_back(std::move(util::mkptr(
                    new flchk::pre_unary_op(pos, "-", std::move(util::mkptr(
                                new flchk::float_literal(pos, "11.11")))))));
    params.push_back(std::move(util::mkptr(
                    new flchk::negation(pos, std::move(util::mkptr(
                                new flchk::int_literal(pos, "21")))))));
    params.push_back(std::move(util::mkptr(new flchk::reference(pos, "dareka_tasukete_kudasai"))));
    flchk::call call1(pos, "leap", std::move(params));

    call0.compile(*scope)->inst(nul_inst_scope);
    call1.compile(*scope)->inst(nul_inst_scope);

    data_tree::expect_one()
        (pos, CALL, "fib", 0, false)
        (pos, CALL, "leap", 4, false)
            (pos, BOOLEAN, "false")
            (pos, PRE_UNARY_OP, "-")
                (pos, FLOATING, "11.11")
            (pos, PRE_UNARY_OP, "!")
                (pos, INTEGER, "21")
            (pos, VAR_REF, "dareka_tasukete_kudasai")
    ;
}
