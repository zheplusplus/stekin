#include <gtest/gtest.h>

#include "test-common.h"
#include "../yy-misc.h"
#include "../../test/phony-errors.h"
#include "../../flowcheck/node-base.h"

using namespace test;

TEST(Syntax, Mix)
{
    yyparse();
    ASSERT_FALSE(error::has_error());

    data_tree::expect_one()
        (misc::position(1), 0, VAR_DEF, "n")
            (misc::position(1), INTEGER, "10")

        (misc::position(3), 0, FUNC_DEF_HEAD_BEGIN, "fib")
            (misc::position(3), 0, IDENTIFIER, "n")
        (misc::position(3), 0, FUNC_DEF_HEAD_END)

            (misc::position(4), 1, FUNC_DEF_HEAD_BEGIN, "add")
            (misc::position(4), 1, FUNC_DEF_HEAD_END)

                (misc::position(5), 2, RETURN)
                    (misc::position(5), BINARY_OP_BEGIN, "+")
                    (misc::position(5), OPERAND)
                        (misc::position(5), FUNC_CALL_BEGIN, "fib")
                        (misc::position(5), ARGUMENT)
                            (misc::position(5), IDENTIFIER, "n")
                        (misc::position(5), FUNC_CALL_END)
                    (misc::position(5), OPERAND)
                        (misc::position(5), FUNC_CALL_BEGIN, "fib")
                        (misc::position(5), ARGUMENT)
                            (misc::position(5), BINARY_OP_BEGIN, "-")
                            (misc::position(5), OPERAND)
                                (misc::position(5), IDENTIFIER, "n")
                            (misc::position(5), OPERAND)
                                (misc::position(5), INTEGER, "1")
                            (misc::position(5), BINARY_OP_END)
                        (misc::position(5), FUNC_CALL_END)
                    (misc::position(5), BINARY_OP_END)

            (misc::position(6), 1, BRANCH_IFNOT)
            (misc::position(6), 1, CONDITION_BEGIN)
                (misc::position(6), BINARY_OP_BEGIN, "<")
                (misc::position(6), OPERAND)
                    (misc::position(6), IDENTIFIER, "n")
                (misc::position(6), OPERAND)
                    (misc::position(6), INTEGER, "2")
                (misc::position(6), BINARY_OP_END)
            (misc::position(6), 1, CONDITION_END)
                (misc::position(7), 2, RETURN)
                        (misc::position(7), FUNC_CALL_BEGIN, "add")
                        (misc::position(7), FUNC_CALL_END)

            (misc::position(8), 1, RETURN)
                (misc::position(8), INTEGER, "1")

            (misc::position(10), 0, BRANCH_IF)
            (misc::position(10), 0, CONDITION_BEGIN)
                (misc::position(10), BINARY_OP_BEGIN, ">")
                (misc::position(10), OPERAND)
                    (misc::position(10), IDENTIFIER, "n")
                (misc::position(10), OPERAND)
                    (misc::position(10), INTEGER, "5")
                (misc::position(10), BINARY_OP_END)
            (misc::position(10), 0, CONDITION_END)
                (misc::position(11), 1, ARITHMETICS)
                    (misc::position(11), FUNC_CALL_BEGIN, "write")
                        (misc::position(11), ARGUMENT)
                        (misc::position(11), FUNC_CALL_BEGIN, "fib")
                            (misc::position(11), ARGUMENT)
                                (misc::position(11), INTEGER, "10")
                        (misc::position(11), FUNC_CALL_END)
                    (misc::position(11), FUNC_CALL_END)
            (misc::position(12), 0, BRANCH_ELSE)
                (misc::position(13), 1, ARITHMETICS)
                    (misc::position(13), FUNC_CALL_BEGIN, "write")
                        (misc::position(13), ARGUMENT)
                            (misc::position(13), BOOLEAN, "false")
                    (misc::position(13), FUNC_CALL_END)

        (misc::position(15), 0, FUNC_DEF_HEAD_BEGIN, "print_10")
        (misc::position(15), 0, FUNC_DEF_HEAD_END)
            (misc::position(16), 1, BRANCH_IF)
            (misc::position(16), 1, CONDITION_BEGIN)
                (misc::position(16), BOOLEAN, "true")
            (misc::position(16), 1, CONDITION_END)
                (misc::position(17), 2, ARITHMETICS)
                    (misc::position(17), FUNC_CALL_BEGIN, "write")
                        (misc::position(17), ARGUMENT)
                            (misc::position(17), PRE_UNARY_OP_BEGIN, "+")
                            (misc::position(17), OPERAND)
                                (misc::position(17), FLOATING, "10.0")
                            (misc::position(17), PRE_UNARY_OP_END)
                    (misc::position(17), FUNC_CALL_END)

        (misc::position(19), 0, VAR_DEF, "x")
            (misc::position(19), IDENTIFIER, "print_10@0")
        (misc::position(20), 0, VAR_DEF, "y")
            (misc::position(20), IDENTIFIER, "fib@1")
    ;
    data_tree::verify();
}
