#include <vector>
#include <gtest/gtest.h>

#include "test-common.h"
#include "../expr-nodes.h"
#include "../filter.h"
#include "../function.h"
#include "../../proto/node-base.h"
#include "../../proto/function.h"
#include "../../proto/global-scope.h"
#include "../../instance/node-base.h"
#include "../../instance/inst-mediate.h"
#include "../../test/phony-errors.h"
#include "../../test/phony-warnings.h"
#include "../../test/common.h"

using namespace test;

typedef flowcheck_test FilterTest;

TEST_F(FilterTest, FilterNormal)
{
    misc::pos_type pos(1);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));

    util::sptr<flchk::binary_op> binary(
            new flchk::binary_op(pos
                               , std::move(util::mkptr(new flchk::IntLiteral(pos, "1")))
                               , "+"
                               , std::move(util::mkptr(new flchk::float_literal(pos, "235.7")))));

    flchk::filter filter0;
    filter0.add_arith(pos, std::move(util::mkptr(new flchk::bool_literal(pos, true))));
    filter0.def_var(pos, "soujirou", std::move(binary));
    filter0.def_var(pos, "iwasaki", std::move(util::mkptr(new flchk::reference(pos, "minami"))));

    util::sptr<flchk::filter> filter_consq(new flchk::filter);
    misc::pos_type pos_consq(100);
    filter_consq->add_arith(pos_consq, std::move(util::mkptr(new flchk::reference(pos_consq, "kobayakawa"))));

    util::sptr<flchk::filter> filter_alter(new flchk::filter);
    misc::pos_type pos_alter(101);
    filter_alter->add_arith(pos_alter, std::move(util::mkptr(new flchk::reference(pos_alter, "yutaka"))));
    filter_alter->add_arith(pos_alter, std::move(util::mkptr(new flchk::reference(pos_alter, "hiyori"))));

    filter0.add_branch(pos
                     , std::move(util::mkptr(new flchk::reference(pos, "tamura")))
                     , std::move(filter_consq)
                     , std::move(filter_alter));
    filter0.add_func_ret_nothing(pos);

    filter0.deliver().compile(*scope);
    scope->deliver().inst(nul_inst_scope);

    EXPECT_FALSE(error::has_error());

    data_tree::expect_one()
        (pos, SCOPE_VAR_DEF, "soujirou")
        (pos, SCOPE_VAR_DEF, "iwasaki")
        (SCOPE_BEGIN)
            (pos, ARITHMETICS)
                (pos, BOOLEAN, "true")
            (pos, VAR_DEF, "soujirou")
                (pos, FLOATING, "236.7")
            (pos, VAR_DEF, "iwasaki")
                (pos, REFERENCE, "minami")
            (pos, BRANCH)
            (pos, REFERENCE, "tamura")
            (SCOPE_BEGIN)
                (pos_consq, ARITHMETICS)
                    (pos_consq, REFERENCE, "kobayakawa")
            (SCOPE_END)
            (SCOPE_BEGIN)
                (pos_alter, ARITHMETICS)
                    (pos_alter, REFERENCE, "yutaka")
                (pos_alter, ARITHMETICS)
                    (pos_alter, REFERENCE, "hiyori")
            (SCOPE_END)
            (pos, RETURN_NOTHING)
        (SCOPE_END)
    ;
}

TEST_F(FilterTest, TerminatedError)
{
    misc::pos_type pos(2);
    misc::pos_type pos_error(200);
    misc::pos_type pos_ignored(201);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));

    flchk::filter filter0;
    filter0.def_var(pos, "patricia", std::move(util::mkptr(new flchk::reference(pos, "martin"))));
    filter0.add_func_ret_nothing(pos);
    filter0.add_func_ret(pos_error, std::move(util::mkptr(new flchk::reference(pos, "patty"))));
    filter0.add_func_ret_nothing(pos_ignored);

    ASSERT_TRUE(error::has_error());
    ASSERT_EQ(1, get_flow_terminated_recs().size());
    ASSERT_EQ(pos_error, get_flow_terminated_recs()[0].this_pos);
    ASSERT_EQ(pos, get_flow_terminated_recs()[0].prev_pos);
}

TEST_F(FilterTest, TerminatedWarningIfConsequence)
{
    misc::pos_type pos(3);
    misc::pos_type pos_warning(300);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));

    flchk::filter filter0;

    util::sptr<flchk::filter> filter_consq(new flchk::filter);
    filter_consq->add_arith(pos, std::move(util::mkptr(new flchk::reference(pos, "ayano"))));
    filter_consq->add_func_ret_nothing(pos_warning);

    util::sptr<flchk::filter> filter_alter(new flchk::filter);
    filter_alter->add_arith(pos, std::move(util::mkptr(new flchk::bool_literal(pos, true))));

    filter0.add_branch(pos
                     , std::move(util::mkptr(new flchk::reference(pos, "minegishi")))
                     , std::move(filter_consq)
                     , std::move(filter_alter));

    EXPECT_FALSE(error::has_error());
    ASSERT_EQ(1, get_consq_branch_terminated().size());
    ASSERT_EQ(pos_warning, get_consq_branch_terminated()[0].pos);
}

TEST_F(FilterTest, TerminatedWarningIfAlternative)
{
    misc::pos_type pos(4);
    misc::pos_type pos_warning(400);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));

    flchk::filter filter0;

    util::sptr<flchk::filter> filter_consq(new flchk::filter);
    filter_consq->add_arith(pos, std::move(util::mkptr(new flchk::IntLiteral(pos, "20110411"))));

    util::sptr<flchk::filter> filter_alter(new flchk::filter);
    filter_alter->add_func_ret(pos_warning, std::move(util::mkptr(new flchk::reference(pos, "kogami"))));

    filter0.add_branch(pos
                     , std::move(util::mkptr(new flchk::reference(pos, "akira")))
                     , std::move(filter_consq)
                     , std::move(filter_alter));

    EXPECT_FALSE(error::has_error());
    ASSERT_EQ(1, get_alter_branch_terminated().size());
    ASSERT_EQ(pos_warning, get_alter_branch_terminated()[0].pos);
}

TEST_F(FilterTest, TerminatedWarningBothBranches)
{
    misc::pos_type pos(5);
    misc::pos_type pos_warning_consq(500);
    misc::pos_type pos_warning_alter(501);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));

    flchk::filter filter0;

    util::sptr<flchk::filter> filter_consq(new flchk::filter);
    filter_consq->add_func_ret(pos_warning_consq, std::move(util::mkptr(new flchk::float_literal(pos, ".1"))));

    util::sptr<flchk::filter> filter_alter(new flchk::filter);
    filter_alter->add_func_ret(pos_warning_alter, std::move(util::mkptr(new flchk::reference(pos, "minoru"))));

    filter0.add_branch(pos
                     , std::move(util::mkptr(new flchk::reference(pos, "shiraishi")))
                     , std::move(filter_consq)
                     , std::move(filter_alter));

    EXPECT_FALSE(error::has_error());
    ASSERT_EQ(1, get_both_branches_terminated().size());
    ASSERT_EQ(pos_warning_consq, get_both_branches_terminated()[0].consq_pos);
    ASSERT_EQ(pos_warning_alter, get_both_branches_terminated()[0].alter_pos);
}

TEST_F(FilterTest, TwoPathBranchFoldedOnFalse)
{
    misc::pos_type pos(6);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));

    util::sptr<flchk::binary_op> binary(
            new flchk::binary_op(pos
                               , std::move(util::mkptr(new flchk::IntLiteral(pos, "2048")))
                               , ">"
                               , std::move(util::mkptr(new flchk::float_literal(pos, "11235.8")))));

    flchk::filter filter0;

    util::sptr<flchk::filter> filter_consq(new flchk::filter);
    misc::pos_type pos_consq(600);
    filter_consq->add_arith(pos_consq, std::move(util::mkptr(new flchk::reference(pos_consq, "yui"))));

    util::sptr<flchk::filter> filter_alter(new flchk::filter);
    misc::pos_type pos_alter(601);
    filter_alter->add_arith(pos_alter, std::move(util::mkptr(new flchk::reference(pos_alter, "narumi"))));

    filter0.add_branch(pos, std::move(binary), std::move(filter_consq), std::move(filter_alter));
    filter0.add_func_ret_nothing(pos);

    filter0.deliver().compile(*scope);
    scope->deliver().inst(nul_inst_scope);

    EXPECT_FALSE(error::has_error());

    data_tree::expect_one()
        (SCOPE_BEGIN)
        (pos_alter, ARITHMETICS)
            (pos_alter, REFERENCE, "narumi")
        (pos, RETURN_NOTHING)
        (SCOPE_END)
    ;
}

TEST_F(FilterTest, TwoPathBranchFoldedOnTrue)
{
    misc::pos_type pos(7);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));

    util::sptr<flchk::binary_op> binary(
            new flchk::binary_op(pos
                               , std::move(util::mkptr(new flchk::IntLiteral(pos, "2048")))
                               , "<"
                               , std::move(util::mkptr(new flchk::float_literal(pos, "11235.8")))));

    flchk::filter filter0;

    util::sptr<flchk::filter> filter_consq(new flchk::filter);
    misc::pos_type pos_consq(700);
    filter_consq->add_arith(pos_consq, std::move(util::mkptr(new flchk::reference(pos_consq, "yui"))));

    util::sptr<flchk::filter> filter_alter(new flchk::filter);
    misc::pos_type pos_alter(701);
    filter_alter->add_arith(pos_alter, std::move(util::mkptr(new flchk::reference(pos_alter, "narumi"))));

    filter0.add_branch(pos, std::move(binary), std::move(filter_consq), std::move(filter_alter));
    filter0.add_func_ret_nothing(pos);

    filter0.deliver().compile(*scope);
    scope->deliver().inst(nul_inst_scope);

    EXPECT_FALSE(error::has_error());

    data_tree::expect_one()
        (SCOPE_BEGIN)
        (pos_consq, ARITHMETICS)
            (pos_consq, REFERENCE, "yui")
        (pos, RETURN_NOTHING)
        (SCOPE_END)
    ;
}

TEST_F(FilterTest, IfNotFoldedOnFalse)
{
    misc::pos_type pos(8);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));

    util::sptr<flchk::binary_op> binary(
            new flchk::binary_op(pos
                               , std::move(util::mkptr(new flchk::IntLiteral(pos, "2048")))
                               , ">"
                               , std::move(util::mkptr(new flchk::float_literal(pos, "11235.8")))));

    flchk::filter filter0;

    util::sptr<flchk::filter> filter_alter(new flchk::filter);
    misc::pos_type pos_alter(801);
    filter_alter->add_arith(pos_alter, std::move(util::mkptr(new flchk::reference(pos_alter, "narumi"))));

    filter0.add_branch_alt_only(pos, std::move(binary), std::move(filter_alter));
    filter0.add_func_ret_nothing(pos);

    filter0.deliver().compile(*scope);
    scope->deliver().inst(nul_inst_scope);

    EXPECT_FALSE(error::has_error());

    data_tree::expect_one()
        (SCOPE_BEGIN)
        (pos_alter, ARITHMETICS)
            (pos_alter, REFERENCE, "narumi")
        (pos, RETURN_NOTHING)
        (SCOPE_END)
    ;
}

TEST_F(FilterTest, IfNotFoldedOnTrue)
{
    misc::pos_type pos(8);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));

    util::sptr<flchk::binary_op> binary(
            new flchk::binary_op(pos
                               , std::move(util::mkptr(new flchk::IntLiteral(pos, "2048")))
                               , "<"
                               , std::move(util::mkptr(new flchk::float_literal(pos, "11235.8")))));

    flchk::filter filter0;

    util::sptr<flchk::filter> filter_alter(new flchk::filter);
    misc::pos_type pos_alter(801);
    filter_alter->add_arith(pos_alter, std::move(util::mkptr(new flchk::reference(pos_alter, "narumi"))));

    filter0.add_branch_alt_only(pos, std::move(binary), std::move(filter_alter));
    filter0.add_func_ret_nothing(pos);

    filter0.deliver().compile(*scope);
    scope->deliver().inst(nul_inst_scope);

    EXPECT_FALSE(error::has_error());

    data_tree::expect_one()
        (SCOPE_BEGIN)
        (pos, RETURN_NOTHING)
        (SCOPE_END)
    ;
}
