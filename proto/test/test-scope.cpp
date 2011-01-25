#include <gtest/gtest.h>

#include "test-common.h"
#include "phony-err-report.h"
#include "../scope.h"
#include "../symbol-table.h"
#include "../func-templ.h"

using namespace test;

struct ScopeTest
    : public proto_test
{
    void SetUp()
    {
        proto_test::SetUp();
        reset_scope();
    }

    void reset_scope()
    {
        symbols.reset(new proto::symbol_table);
        scope.reset(new proto::scope(*symbols));
        inst_scope.reset(new inst::scope(util::sref<inst::function>(NULL)
                                       , util::sref<inst::symbol_table>(NULL)));
    }

    util::sptr<proto::symbol_table> symbols;
    util::sptr<proto::scope> scope;
    util::sptr<inst::scope> inst_scope;
};

TEST_F(ScopeTest, ExprNodesCreation)
{
    misc::pos_type pos(1);

    scope->make_bool(pos, true)->inst(*inst_scope)->typeof();
    scope->make_bool(pos, false)->inst(*inst_scope)->typeof();
    scope->make_int(pos, "0")->inst(*inst_scope)->typeof();
    scope->make_int(pos, "2048")->inst(*inst_scope)->typeof();
    scope->make_float(pos, "21.36")->inst(*inst_scope)->typeof();
    scope->make_float(pos, "0.0")->inst(*inst_scope)->typeof();
    scope->make_ref(pos, "zero")->inst(*inst_scope)->typeof();
    scope->make_ref(pos, "one")->inst(*inst_scope)->typeof();
    scope->make_binary(pos, std::move(scope->make_int(pos, "1")), "+", std::move(scope->make_bool(pos, true)))
         ->inst(*inst_scope)
         ->typeof();
    scope->make_binary(pos, std::move(scope->make_int(pos, "4")), "<=", std::move(scope->make_bool(pos, false)))
         ->inst(*inst_scope)
         ->typeof();
    scope->make_pre_unary(pos, "-", std::move(scope->make_float(pos, "0.9")))->inst(*inst_scope)->typeof();
    scope->make_pre_unary(pos, "+", std::move(scope->make_float(pos, "1.6")))->inst(*inst_scope)->typeof();
    scope->make_conj(pos, std::move(scope->make_int(pos, "25")), std::move(scope->make_bool(pos, true)))
         ->inst(*inst_scope)
         ->typeof();
    scope->make_disj(pos, std::move(scope->make_bool(pos, false)), std::move(scope->make_int(pos, "36")))
         ->inst(*inst_scope)
         ->typeof();
    scope->make_nega(pos, std::move(scope->make_int(pos, "49")))->inst(*inst_scope)->typeof();
    ASSERT_FALSE(proto::has_error());

    data_tree::expect_one()
        (BOOLEAN, "true")
        (BOOLEAN, "false")
        (INTEGER, "0")
        (INTEGER, "2048")
        (FLOATING, "21.36")
        (FLOATING, "0")
        (pos, QUERY_VAR, "zero")
        (REFERENCE)
        (pos, QUERY_VAR, "one")
        (REFERENCE)
            (INTEGER, "1")
            (BOOLEAN, "true")
        (pos, QUERY_BINARY_OP, "+")
        (BINARY_OP)
            (INTEGER, "1")
            (BOOLEAN, "true")

            (INTEGER, "4")
            (BOOLEAN, "false")
        (pos, QUERY_BINARY_OP, "<=")
        (BINARY_OP)
            (INTEGER, "4")
            (BOOLEAN, "false")

            (FLOATING, "0.9")
        (pos, QUERY_PRE_UNARY_OP, "-")
        (PRE_UNARY_OP)
            (FLOATING, "0.9")

            (FLOATING, "1.6")
        (pos, QUERY_PRE_UNARY_OP, "+")
        (PRE_UNARY_OP)
            (FLOATING, "1.6")

        (CONJUNCTION)
            (INTEGER, "25")
            (BOOLEAN, "true")

        (DISJUNCTION)
            (BOOLEAN, "false")
            (INTEGER, "36")

        (NEGATION)
            (INTEGER, "49")
    ;
}

TEST_F(ScopeTest, Symbol)
{
    misc::pos_type pos(2);
    scope->add_arith(pos, std::move(scope->make_ref(pos, "four")));
    ASSERT_FALSE(proto::has_error());
    scope->def_var(pos, "four", std::move(scope->make_bool(pos, true)));
    ASSERT_TRUE(proto::has_error());
    ASSERT_EQ(1, get_invalid_refs().size());
    ASSERT_EQ(pos, get_invalid_refs()[0].def_pos);
    ASSERT_EQ(1, get_invalid_refs()[0].ref_positions.size());
    ASSERT_EQ(pos, get_invalid_refs()[0].ref_positions[0]);
    clear_err();

    scope->decl_func(pos, "five", std::vector<std::string>({ "a", "b" }));
    ASSERT_FALSE(proto::has_error());
    scope->get_symbols()->def_func(pos, "five", std::vector<std::string>({ "m", "n" }));
    ASSERT_TRUE(proto::has_error());
    ASSERT_EQ(1, get_local_func_redefs().size());
    ASSERT_EQ(pos, get_local_func_redefs()[0].this_def_pos);
    ASSERT_EQ(pos, get_local_func_redefs()[0].prev_def_pos);
    ASSERT_EQ("five", get_local_func_redefs()[0].name);
    ASSERT_EQ(2, get_local_func_redefs()[0].param_count);
}

TEST_F(ScopeTest, TerminateStatus)
{
    misc::pos_type pos(3);
    ASSERT_EQ(proto::NO_EXPLICIT_TERMINATION, scope->termination());
    scope->add_arith(pos, std::move(scope->make_ref(pos, "six")));
    ASSERT_EQ(proto::NO_EXPLICIT_TERMINATION, scope->termination());

    scope->add_func_ret(pos, std::move(scope->make_ref(pos, "six")));
    ASSERT_EQ(proto::RETURN_NO_VOID, scope->termination());
    scope->add_arith(pos, std::move(scope->make_ref(pos, "six")));
    ASSERT_EQ(proto::RETURN_NO_VOID, scope->termination());
    scope->add_func_ret_nothing(pos);
    ASSERT_EQ(proto::RETURN_VOID, scope->termination());
    scope->add_arith(pos, std::move(scope->make_ref(pos, "six")));
    ASSERT_EQ(proto::RETURN_VOID, scope->termination());

    reset_scope();
    scope->add_func_ret_nothing(pos);
    ASSERT_EQ(proto::RETURN_VOID, scope->termination());
    scope->add_arith(pos, std::move(scope->make_ref(pos, "seven")));
    ASSERT_EQ(proto::RETURN_VOID, scope->termination());
    scope->add_func_ret(pos, std::move(scope->make_ref(pos, "seven")));
    ASSERT_EQ(proto::RETURN_VOID, scope->termination());
    scope->add_arith(pos, std::move(scope->make_ref(pos, "seven")));
    ASSERT_EQ(proto::RETURN_VOID, scope->termination());

    util::sptr<proto::scope> sub_scope0(NULL);
    reset_scope();
    sub_scope0 = std::move(scope->create_loop_scope());
    ASSERT_EQ(proto::NO_EXPLICIT_TERMINATION, sub_scope0->termination());
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::NO_EXPLICIT_TERMINATION, scope->termination());

    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret(pos, std::move(sub_scope0->make_bool(pos, true)));
    ASSERT_EQ(proto::RETURN_NO_VOID, sub_scope0->termination());
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_NO_VOID, scope->termination());

    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret_nothing(pos);
    ASSERT_EQ(proto::RETURN_VOID, sub_scope0->termination());
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_VOID, scope->termination());

    scope->add_func_ret(pos, std::move(scope->make_ref(pos, "eight")));
    ASSERT_EQ(proto::PARTIAL_RETURN_VOID, scope->termination());
    scope->add_func_ret_nothing(pos);
    ASSERT_EQ(proto::RETURN_VOID, scope->termination());

    reset_scope();
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret(pos, std::move(sub_scope0->make_bool(pos, true)));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    scope->add_func_ret(pos, std::move(scope->make_ref(pos, "eight")));
    ASSERT_EQ(proto::RETURN_NO_VOID, scope->termination());
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret(pos, std::move(sub_scope0->make_bool(pos, true)));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::RETURN_NO_VOID, scope->termination());
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret_nothing(pos);
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_VOID, scope->termination());

    reset_scope();
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret(pos, std::move(sub_scope0->make_bool(pos, true)));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    scope->add_func_ret_nothing(pos);
    ASSERT_EQ(proto::RETURN_VOID, scope->termination());
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret(pos, std::move(sub_scope0->make_bool(pos, true)));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::RETURN_VOID, scope->termination());
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret_nothing(pos);
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::RETURN_VOID, scope->termination());
    sub_scope0 = std::move(scope->create_loop_scope());
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::RETURN_VOID, scope->termination());
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret(pos, std::move(scope->make_ref(pos, "nine")));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::RETURN_VOID, scope->termination());

    reset_scope();
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret(pos, std::move(sub_scope0->make_bool(pos, true)));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    sub_scope0 = std::move(scope->create_loop_scope());
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_NO_VOID, scope->termination());

    reset_scope();
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret_nothing(pos);
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    sub_scope0 = std::move(scope->create_loop_scope());
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_VOID, scope->termination());

    reset_scope();
    scope->add_func_ret(pos, std::move(scope->make_bool(pos, true)));
    sub_scope0 = std::move(scope->create_loop_scope());
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::RETURN_NO_VOID, scope->termination());

    reset_scope();
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret_nothing(pos);
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_VOID, scope->termination());
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret_nothing(pos);
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_VOID, scope->termination());
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret(pos, std::move(sub_scope0->make_ref(pos, "ten")));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_VOID, scope->termination());

    reset_scope();
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret(pos, std::move(sub_scope0->make_ref(pos, "eleven")));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_NO_VOID, scope->termination());
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope0->add_func_ret(pos, std::move(sub_scope0->make_ref(pos, "twelve")));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_NO_VOID, scope->termination());

    util::sptr<proto::scope> sub_scope1(NULL);
    util::sptr<proto::scope> sub_scope2(NULL);

    reset_scope();
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope1 = std::move(sub_scope0->create_branch_scope());
    sub_scope2 = std::move(sub_scope0->create_branch_scope());
    sub_scope1->add_func_ret(pos, std::move(sub_scope1->make_ref(pos, "thirteen")));
    sub_scope0->add_branch(pos
                         , std::move(sub_scope0->make_bool(pos, false))
                         , std::move(sub_scope1)
                         , std::move(sub_scope2));
    ASSERT_EQ(proto::PARTIAL_RETURN_NO_VOID, sub_scope0->termination());
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_NO_VOID, scope->termination());
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope1 = std::move(sub_scope0->create_branch_scope());
    sub_scope2 = std::move(sub_scope0->create_branch_scope());
    sub_scope1->add_func_ret(pos, std::move(sub_scope1->make_ref(pos, "fourteen")));
    sub_scope0->add_branch(pos
                         , std::move(sub_scope0->make_bool(pos, false))
                         , std::move(sub_scope1)
                         , std::move(sub_scope2));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_NO_VOID, scope->termination());

    scope->add_func_ret(pos, std::move(scope->make_ref(pos, "fifteen")));
    ASSERT_EQ(proto::RETURN_NO_VOID, scope->termination());
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope1 = std::move(sub_scope0->create_branch_scope());
    sub_scope2 = std::move(sub_scope0->create_branch_scope());
    sub_scope1->add_func_ret(pos, std::move(sub_scope1->make_ref(pos, "sixteen")));
    sub_scope0->add_branch(pos
                         , std::move(sub_scope0->make_bool(pos, false))
                         , std::move(sub_scope1)
                         , std::move(sub_scope2));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::RETURN_NO_VOID, scope->termination());

    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope1 = std::move(sub_scope0->create_branch_scope());
    sub_scope2 = std::move(sub_scope0->create_branch_scope());
    sub_scope1->add_func_ret_nothing(pos);
    sub_scope0->add_branch(pos
                         , std::move(sub_scope0->make_bool(pos, false))
                         , std::move(sub_scope1)
                         , std::move(sub_scope2));
    ASSERT_EQ(proto::PARTIAL_RETURN_VOID, sub_scope0->termination());
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_VOID, scope->termination());
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope1 = std::move(sub_scope0->create_branch_scope());
    sub_scope2 = std::move(sub_scope0->create_branch_scope());
    sub_scope1->add_func_ret_nothing(pos);
    sub_scope0->add_branch(pos
                         , std::move(sub_scope0->make_bool(pos, false))
                         , std::move(sub_scope1)
                         , std::move(sub_scope2));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_VOID, scope->termination());

    scope->add_func_ret_nothing(pos);
    ASSERT_EQ(proto::RETURN_VOID, scope->termination());
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope1 = std::move(sub_scope0->create_branch_scope());
    sub_scope2 = std::move(sub_scope0->create_branch_scope());
    sub_scope1->add_func_ret(pos, std::move(sub_scope1->make_ref(pos, "seventeen")));
    sub_scope0->add_branch(pos
                         , std::move(sub_scope0->make_bool(pos, false))
                         , std::move(sub_scope1)
                         , std::move(sub_scope2));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::RETURN_VOID, scope->termination());
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope1 = std::move(sub_scope0->create_branch_scope());
    sub_scope2 = std::move(sub_scope0->create_branch_scope());
    sub_scope1->add_func_ret_nothing(pos);
    sub_scope0->add_branch(pos
                         , std::move(sub_scope0->make_bool(pos, false))
                         , std::move(sub_scope1)
                         , std::move(sub_scope2));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::RETURN_VOID, scope->termination());

    reset_scope();
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope1 = std::move(sub_scope0->create_branch_scope());
    sub_scope2 = std::move(sub_scope0->create_branch_scope());
    sub_scope1->add_func_ret_nothing(pos);
    sub_scope0->add_branch(pos
                         , std::move(sub_scope0->make_bool(pos, false))
                         , std::move(sub_scope1)
                         , std::move(sub_scope2));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_VOID, scope->termination());

    reset_scope();
    scope->add_func_ret(pos, std::move(scope->make_ref(pos, "eighteen")));
    sub_scope0 = std::move(scope->create_loop_scope());
    sub_scope1 = std::move(sub_scope0->create_branch_scope());
    sub_scope2 = std::move(sub_scope0->create_branch_scope());
    sub_scope1->add_func_ret_nothing(pos);
    sub_scope0->add_branch(pos
                         , std::move(sub_scope0->make_bool(pos, false))
                         , std::move(sub_scope1)
                         , std::move(sub_scope2));
    scope->add_loop(pos, std::move(scope->make_bool(pos, true)), std::move(sub_scope0));
    ASSERT_EQ(proto::PARTIAL_RETURN_VOID, scope->termination());

    ASSERT_FALSE(proto::has_error());
}
