#include <gtest/gtest.h>

#include "test-common.h"
#include "../scope.h"
#include "../symbol-table.h"
#include "../function.h"
#include "../stmt-nodes.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"

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
        ext_symbols.reset(new proto::symbol_table);
        scope.reset(new proto::general_scope(*ext_symbols));
        inst_scope.reset(new phony_func);
    }

    util::sptr<proto::scope> mkscope() const
    {
        return std::move(scope->create_branch_scope());
    }

    util::sptr<proto::symbol_table> ext_symbols;
    util::sptr<proto::general_scope> scope;
    util::sptr<inst::scope> inst_scope;
};

TEST_F(ScopeTest, ExprNodesCreation)
{
    misc::position pos(1);

    scope->make_bool(pos, true)->inst(*inst_scope)->typeof();
    scope->make_bool(pos, false)->inst(*inst_scope)->typeof();
    scope->make_int(pos, 0)->inst(*inst_scope)->typeof();
    scope->make_int(pos, 2048)->inst(*inst_scope)->typeof();
    scope->make_float(pos, 21.36)->inst(*inst_scope)->typeof();
    scope->make_float(pos, 0.0)->inst(*inst_scope)->typeof();
    scope->make_ref(pos, "zero")->inst(*inst_scope)->typeof();
    scope->make_ref(pos, "one")->inst(*inst_scope)->typeof();
    scope->make_binary(pos, std::move(scope->make_int(pos, 1)), "+", std::move(scope->make_bool(pos, true)))
         ->inst(*inst_scope)
         ->typeof();
    scope->make_binary(pos, std::move(scope->make_int(pos, 4)), "<=", std::move(scope->make_bool(pos, false)))
         ->inst(*inst_scope)
         ->typeof();
    scope->make_pre_unary(pos, "-", std::move(scope->make_float(pos, 0.9)))->inst(*inst_scope)->typeof();
    scope->make_pre_unary(pos, "+", std::move(scope->make_float(pos, 1.6)))->inst(*inst_scope)->typeof();
    scope->make_conj(pos, std::move(scope->make_int(pos, 25)), std::move(scope->make_bool(pos, true)))
         ->inst(*inst_scope)
         ->typeof();
    scope->make_disj(pos, std::move(scope->make_bool(pos, false)), std::move(scope->make_int(pos, 36)))
         ->inst(*inst_scope)
         ->typeof();
    scope->make_nega(pos, std::move(scope->make_int(pos, 49)))->inst(*inst_scope)->typeof();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
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
    misc::position pos(2);
    scope->addStmt(std::move(util::mkptr(new proto::Arithmetics(pos
                                                               , std::move(scope->make_ref(pos, "four"))))));
    ASSERT_FALSE(error::hasError());
    scope->defVar(pos, "four");
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, get_invalid_refs().size());
    ASSERT_EQ(pos, get_invalid_refs()[0].def_pos);
    ASSERT_EQ(1, get_invalid_refs()[0].ref_positions.size());
    ASSERT_EQ(pos, get_invalid_refs()[0].ref_positions[0]);
}
